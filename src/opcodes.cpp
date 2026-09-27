#include <gameboy.hpp>

uint16_t Gameboy::emulate_inst() {
    opcode = mmu.read(reg.PC);
    address = reg.PC;
    reg.PC++;

    if (prefixed) {
        prefixed = false;
        switch(opcode) {
            default:
                return 0;
        }
    }

    switch(opcode) {
        default:
                return 0;
    }
}