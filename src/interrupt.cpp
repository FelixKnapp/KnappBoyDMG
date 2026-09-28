#include <gameboy.hpp>

typedef enum {
    VBLANK = 0x40,
    LCD    = 0x48,
    TIMER  = 0x50,
    SERIAL = 0x58,
    JOYPAD = 0x60
} Innterrupt_t;

int Gameboy::handle_interrupt() {
    int return_value = 0;
    if(!reg.IME) return 0;
    reg.IME = false;

    Innterrupt_t interrupt;
    uint8_t interrupts_count = 0;

    uint8_t reg_IE = mmu.read(0xFFFF);
    uint8_t reg_IF = mmu.read(0xFF0F);

    bool vblank = (reg_IF & 0x1) && (reg_IE & 0x1);
    bool lcd = ((reg_IF >> 1)  & 0x1) && ((reg_IE >> 1) & 0x1);
    bool timer = ((reg_IF >> 2)  & 0x1) && ((reg_IE >> 2) & 0x1);
    bool serial = ((reg_IF >> 3)  & 0x1) && ((reg_IE >> 3) & 0x1);
    bool joypad = ((reg_IF >> 4)  & 0x1) && ((reg_IE >> 4) & 0x1);

    if(vblank) {
        interrupt = VBLANK;
    } else if(lcd) {
        interrupt = LCD;
    } else if(timer) {
        interrupt = TIMER;
    } else if(serial) {
        interrupt = SERIAL;
    } else if(joypad) {
        interrupt = JOYPAD;
    } else {
        return return_value;
    }

    return_value += 8;
    push_word(reg.PC);
    return_value += 8;

    reg.PC = interrupt;

    return_value += 4;
    return return_value;
}
