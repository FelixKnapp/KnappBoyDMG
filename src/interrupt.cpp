#include <gameboy.hpp>

typedef enum {
    VBLANK = 0x40,
    LCD    = 0x48,
    TIMER  = 0x50,
    SERIAL = 0x58,
    JOYPAD = 0x60,
    NONE   = 0x100
} Innterrupt_t;

int Gameboy::handle_interrupt() {
    int return_value = 0;
    if(!reg.IME) return 0;
    reg.IME = false;
    fprintf(debug_output, "-------\n");
    fprintf(debug_output, "Handling Interrupt: ");
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
        fprintf(debug_output, "VBLANK\n");
        interrupt = VBLANK;
        reg_IF &= ~0b1;
    } else if(lcd) {
        fprintf(debug_output, "LCD\n");
        interrupt = LCD;
        reg_IF &= ~0b10;
    } else if(timer) {
        fprintf(debug_output, "TIMER\n");
        interrupt = TIMER;
        reg_IF &= ~0b100;
    } else if(serial) {
        fprintf(debug_output, "SERIAL\n");
        interrupt = SERIAL;
        reg_IF &= ~0b1000;
    } else if(joypad) {
        fprintf(debug_output, "JOYPAD\n");
        interrupt = JOYPAD;
        reg_IF &= ~0b10000;
    } else {
        fprintf(debug_output, "NONE\n");
        fprintf(debug_output, "-------\n");
        return return_value;
    }

    return_value += 8;
    push_word(reg.PC);
    return_value += 8;

    reg.PC = interrupt;

    return_value += 4;
    mmu.write_io(0xFF0F, reg_IF);

    fprintf(debug_output, "-------\n");

    return return_value;
}
