#ifndef __HARDWARE_H
#define __HARDWARE_H
#ifdef __cplusplus
extern "C" {
#endif
	
#include "hk32f030m.h" 
	
#define SOFT_DELAY  Delay(0x0FFFFF);
#define SOFT_DELAY_SHORT  Delay(0x0FFFF);
#define SOFT_DELAY_VERY_SHORT Delay(0x0FFF);
#define SOFT_DELAY_EXTREMELY_SHORT Delay(0x080);

/* The method of directly manipulating registers controls IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //output is high		
#define digitalLo(p,i)		 {p->BRR=i;}	 //output is low
#define digitalToggle(p,i) {p->ODR ^=i;} //toggle the output
#define digitalRead GPIO_ReadInputDataBit

//pin definitions
#define btnA_pin GPIO_Pin_6
#define btnB_pin GPIO_Pin_5

#define btnA_port GPIOD
#define btnB_port GPIOD

#define btnA btnA_port,btnA_pin
#define btnB btnB_port,btnB_pin

#define btnA_EXTI_port EXTI_PortSourceGPIOD
#define btnB_EXTI_port EXTI_PortSourceGPIOD

#define btnA_EXTI_pin GPIO_PinSource6
#define btnB_EXTI_pin GPIO_PinSource5

#define dpad_pin GPIO_Pin_4
#define dpad_port GPIOC
#define dpad_adc ADC1
#define dpad_adc_channel ADC_Channel_2
#define dpad_adc_irq ADC1_IRQn
#define dpad_adc_irq_handler ADC1_IRQHandler

void Delay(__IO uint32_t nCount);

void set_gpio_mode(GPIO_TypeDef *port, uint16_t pin, GPIOMode_TypeDef mode);

void init_gpio_clocks();

void init_matrix_gpios();

void init_button_gpios();

void init_dpad_gpio();

void control_matrix_led(uint8_t x, uint8_t y, uint8_t on);

void update_buttons();

void sleep();

#ifdef __cplusplus
}
#endif
#endif