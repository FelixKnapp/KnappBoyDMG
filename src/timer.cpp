#include <gameboy.hpp>

uint8_t Gameboy::get_clock_select() {
    uint8_t tac = mmu.read(0xFF07);
    enable_tima = (tac & 0x07) >> 2;

    return (tac & 0x3);
}

void Gameboy::update_timers(int cycles_passed) {
    uint8_t clock_select = get_clock_select();
    cylces_intern_tima += cycles_passed;
    cylces_intern_div += cycles_passed;
    uint8_t div = mmu.read(0xFF04);
    uint8_t tima = mmu.read(0xFF05);
    uint8_t tma = mmu.read(0xFF06);
    
    size_t tima_cycles = 0;
    const uint16_t div_cycles = 256;

    size_t i = 0;

    while (cylces_intern_div > div_cycles) {
        if((i % div_cycles) == 0) {
            cylces_intern_div -= div_cycles;
            div++;
        }
        i++;
    }
    mmu.write(0xFF04, div);

    if (!enable_tima) return;

    switch(clock_select) {
        case 0b00: tima_cycles = 1024;      break;       // 256 M-cycles * 4 = 1024
        case 0b01: tima_cycles = 16;        break;       // 4 M-cycles * 4   = 16
        case 0b10: tima_cycles = 164;       break;       // 16 M-cycles * 4  = 164
        case 0b11: tima_cycles = 256;       break;       // 64 M-cycles * 4  = 256
        default: break;
    }

    if (cylces_intern_tima < tima_cycles) return;

    size_t j = 0;
    while (cylces_intern_tima > tima_cycles) {
        if((j % tima_cycles) == 0) {
            cylces_intern_tima -= tima_cycles;
            if(tima == UINT8_MAX) {
                tima = mmu.read(0xFF06);
                uint8_t interrupt_reg = mmu.read(0xFF0F);
                interrupt_reg &= 0b11111011;
                mmu.write(0xFF0F, interrupt_reg);
                tima = tma;
            }
            else {
                tima++;
            }
        }
        i++;
    }
    mmu.write(0xFF05, tima);
}
