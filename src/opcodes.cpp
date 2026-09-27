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
        
        //* ____MISC____
        case 0x00: 
            // NOP      Cycles: 4       Length: 1
            return 4;

        
        //* ____JP____
        case 0xC3: {
            // JP a16   Cycles: 16      Length: 1
            uint16_t a16 = fetch_word();
            reg.PC = a16;
            return 16;
        }

        default:
                return 0;
    }
}