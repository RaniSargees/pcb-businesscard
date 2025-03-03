#include "hk32f030m.h"
#include "bitfont.h"

uint8_t font[40][4] = {
	{ // SPACE
		0b000,
		0b000,
		0b000,
		0b000
	},
	{ //A
		0b111,
		0b101,
		0b111,
		0b101
	},
	{ //B
		0b110,
		0b110,
		0b101,
		0b111
	},
	{ //C
		0b111,
		0b100,
		0b100,
		0b111
	},
	{ //D
		0b110,
		0b101,
		0b101,
		0b110
	},
	{ //E
		0b111,
		0b100,
		0b110,
		0b111
	},
	{ //F
		0b111,
		0b100,
		0b110,
		0b100
	},
	{ //G
		0b111,
		0b100,
		0b101,
		0b111
	},
	{ //H
		0b101,
		0b101,
		0b111,
		0b101
	},
	{ //I
		0b111,
		0b010,
		0b010,
		0b111
	},
	{ //J
		0b011,
		0b001,
		0b101,
		0b111
	},
	{ //K
		0b101,
		0b110,
		0b101,
		0b101
	},
	{ //L
		0b100,
		0b100,
		0b100,
		0b111
	},
	{ //M
		0b111,
		0b111,
		0b101,
		0b101
	},
	{ //N
		0b111,
		0b101,
		0b101,
		0b101
	},
	{ //O
		0b111,
		0b101,
		0b101,
		0b111
	},
	{ //P
		0b111,
		0b101,
		0b111,
		0b100
	},
	{ //Q
		0b111,
		0b101,
		0b101,
		0b110
	},
	{ //R
		0b111,
		0b101,
		0b110,
		0b101
	},
	{ //S
		0b011,
		0b110,
		0b001,
		0b110
	},
	{ //T
		0b111,
		0b010,
		0b010,
		0b010
	},
	{ //U
		0b101,
		0b101,
		0b101,
		0b111
	},
	{ //V
		0b101,
		0b101,
		0b010,
		0b010
	},
	{ //W
		0b101,
		0b101,
		0b111,
		0b111
	},
	{ //X
		0b101,
		0b010,
		0b010,
		0b101
	},
	{ //Y
		0b101,
		0b010,
		0b010,
		0b010
	},
	{ //Z
		0b111,
		0b011,
		0b100,
		0b111
	},
	{ //@
		0b011,
		0b110,
		0b101,
		0b111
	},
	{ //:
		0b010,
		0b000,
		0b000,
		0b010
	},
	{ //.
		0b000,
		0b000,
		0b000,
		0b010
	},
	{ //0
		0b111,
		0b101,
		0b101,
		0b111
	},
	{ //1
		0b110,
		0b010,
		0b010,
		0b111
	},
	{ //2
		0b110,
		0b001,
		0b010,
		0b111
	},
	{ //3
		0b111,
		0b001,
		0b011,
		0b111
	},
	{ //4
		0b101,
		0b111,
		0b001,
		0b001
	},
	{ //5
		0b111,
		0b110,
		0b001,
		0b111
	}, 
	{ //6
		0b100,
		0b111,
		0b101,
		0b111
	},
	{ //7
		0b111,
		0b001,
		0b010,
		0b100
	},
	{ //8
		0b011,
		0b011,
		0b110,
		0b110
	}, 
	{ //9
		0b111,
		0b101,
		0b111,
		0b001
	}
};