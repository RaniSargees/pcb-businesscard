#include "hk32f030m.h" 
#include "hk32f030m_it.h" 
#include "hk32f030m_pwr.h"
#include "hk32f030m_exti.h"
#include "hardware.h"

//buttons are stored in one byte, pattern 0b00UDLRBA
//with only one directional key permitted at a time
uint8_t buttons = 0;
uint8_t button_change = 0;

uint8_t analog  = 0;

void Delay(__IO uint32_t nCount) { //simple delay function
	for(; nCount != 0; nCount--);
}

uint16_t matrix_pins[11] = {
	GPIO_Pin_1,
	GPIO_Pin_2,
	GPIO_Pin_3,
	GPIO_Pin_4,
	GPIO_Pin_4,
	GPIO_Pin_3,
	GPIO_Pin_5,
	GPIO_Pin_6,
	GPIO_Pin_1,
	GPIO_Pin_2,
	GPIO_Pin_3,
};
GPIO_TypeDef * matrix_ports[11] = {
	GPIOA,
	GPIOA,
	GPIOA,
	GPIOD,
	GPIOB,
	GPIOC,
	GPIOC,
	GPIOC,
	GPIOD,
	GPIOD,
	GPIOD,
};

void set_gpio_mode(GPIO_TypeDef *port, uint16_t pin, GPIOMode_TypeDef mode) {
	/*-- GPIO Mode Configuration --*/
	uint32_t pinpos = 0x00, pos = 0x00 , currentpin = 0x00;

  for (pinpos = 0x00; pinpos < 0x10; pinpos++) {
    pos = ((uint32_t)0x01) << pinpos;

    /* Get the port pins position */
    currentpin = (pin) & pos;

    if (currentpin == pos) {
      port->MODER  &= ~(GPIO_MODER_MODER0 << (pinpos * 2));
      port->MODER |= (((uint32_t)mode) << (pinpos * 2));
    }
  }
}

void init_gpio_clocks() {
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOC, ENABLE);
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOD, ENABLE);
}

void init_matrix_gpios() {	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT; //output
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //push-pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; //slow

	for (uint8_t i = 0; i<11; ++i) {
		GPIO_InitStructure.GPIO_Pin = matrix_pins[i];
		GPIO_Init(matrix_ports[i], &GPIO_InitStructure); //initialize
		GPIO_ResetBits(matrix_ports[i], matrix_pins[i]); //clear
		set_gpio_mode(matrix_ports[i], matrix_pins[i], GPIO_Mode_IN); //change to high-Z (input)
	}
}

void init_button_gpios() {
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_10MHz;
	
	GPIO_InitStructure.GPIO_Pin = btnA_pin;
	GPIO_Init(btnA_port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = btnB_pin;
	GPIO_Init(btnB_port, &GPIO_InitStructure);
}

void init_dpad_gpio() {
	//adc gpio config
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_InitStructure.GPIO_Pin = dpad_pin;
	GPIO_Init(dpad_port, &GPIO_InitStructure);
	
	//adc mode config
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_ADC, ENABLE );	
	ADC_DeInit(dpad_adc);
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_StructInit(&ADC_InitStructure);
	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
	
	ADC_Init(dpad_adc,&ADC_InitStructure);
	
	ADC_ChannelConfig(dpad_adc, dpad_adc_channel , ADC_SampleTime_239_5Cycles);
	ADC_GetCalibrationFactor(dpad_adc);
	ADC_Cmd(dpad_adc, ENABLE);
	
	while(!ADC_GetFlagStatus(dpad_adc, ADC_FLAG_ADRDY));
	
	ADC_StartOfConversion(dpad_adc);
}

void control_matrix_led(uint8_t x, uint8_t y, uint8_t on) {
	static uint8_t current_x = 0;
	static uint8_t current_y = 0;
	
	if (x >= y){++x;} //charlieplexing is fun!

	//reset previous light gpio
	GPIO_ResetBits(matrix_ports[current_x], matrix_pins[current_x]);
	GPIO_ResetBits(matrix_ports[current_y], matrix_pins[current_y]);
	
	//set previous light gpio to high-Z
	set_gpio_mode(matrix_ports[current_x], matrix_pins[current_x], GPIO_Mode_IN);
	set_gpio_mode(matrix_ports[current_y], matrix_pins[current_y], GPIO_Mode_IN);
	
	//update current gpios
	current_x = x;
	current_y = y;
	
	if (!on) {return;}
	
	//set new light gpio to output
	set_gpio_mode(matrix_ports[x], matrix_pins[x], GPIO_Mode_OUT);
	set_gpio_mode(matrix_ports[y], matrix_pins[y], GPIO_Mode_OUT);
	
	//place positive voltage on y line, ground on x line
	digitalLo(matrix_ports[x], matrix_pins[x]);
	digitalHi(matrix_ports[y], matrix_pins[y]);
}

void update_buttons() {
	uint8_t old = buttons;
	
	buttons = (~digitalRead(btnA) & 0b1);
	buttons|= (~digitalRead(btnB) & 0b1) << 1;
		
	if (ADC_GetFlagStatus(dpad_adc, ADC_FLAG_EOC)) {
		ADC_ClearFlag(dpad_adc, ADC_FLAG_EOC);
		analog = (uint8_t) (ADC_GetConversionValue(dpad_adc) >> 7);
			
		buttons |= ((analog & 0b11110) == 0b10000) << 5; //up
		buttons |= ((analog & 0b11111) == 0b01111) << 5; //up again (it's marginal)
		buttons |= ((analog & 0b11111) == 0b10101) << 4; //down
		buttons |= ((analog & 0b11110) == 0b10010) << 3; //left
		buttons |= ((analog & 0b11110) == 0b01010) << 2; //right
	}
	
	button_change = old ^ buttons;
}

void sleep() {
	//set up wakeup interrupt
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannelPriority  = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	
	//init btnA to EXTI1
	SYSCFG_EXTILineConfig(btnA_EXTI_port, btnA_EXTI_pin);
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_Init(&EXTI_InitStructure);
	
	//init btnB to EXTI2
	//SYSCFG_EXTILineConfig(btnB_EXTI_port, btnB_EXTI_pin);
	//EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	//EXTI_Init(&EXTI_InitStructure);

	//sleep and wait for interrupt
	//PWR_EnterStopMode(PWR_Regulator_LowPower, PWR_Entry_WFI);

	//disable wakeup interrupt
	EXTI_DeInit();
	NVIC_DisableIRQ(EXTI1_IRQn);
	NVIC_DisableIRQ(EXTI2_IRQn);
}