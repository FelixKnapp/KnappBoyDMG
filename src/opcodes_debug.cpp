#include <gameboy.hpp>

bool Gameboy::emulate_inst_debug() {

    fprintf(debug_output, "Inst NR: %d, Address: 0x%04X, Opcode: 0x%02X, Desc: ", debug_lines++, address, opcode);

    switch(opcode) 
    {
        // MISC
        case 0x00: 
            fprintf(debug_output, "NOP");
            break;

        // JP
        case 0xC3:
            fprintf(debug_output, "JP a16");
            break;

        default:
            fprintf(debug_output, "Unknown or unimplemented opcode\n");
            printf("Unkwnon Opcode\n");
            return false;
    }
    fprintf(debug_output, "\n");
    return true;
}
