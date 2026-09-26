#pragma once

class registers_t {
public:
	struct {
		union {
			struct {
				unsigned char F;
				unsigned char A;
			};
			unsigned short PSW;
		};
	};
	
	struct {
		union {
			struct {
				unsigned char C;
				unsigned char B;
			};
			unsigned short BC;
		};
	};
	
	struct {
		union {
			struct {
				unsigned char E;
				unsigned char D;
			};
			unsigned short DE;
		};
	};
	
	struct {
		union {
			struct {
				unsigned char L;
				unsigned char H;
			};
			unsigned short HL;
		};
	};
	
	unsigned short SP;
	unsigned short PC;
    bool IME;
};
