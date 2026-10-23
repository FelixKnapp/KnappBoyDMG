#include <gameboy.hpp>

bool Gameboy::emulate_inst_debug() {

    fprintf(debug_output, "Inst NR: %05d, Address: 0x%04X, Opcode: 0x%02X, Desc: ", debug_lines++, address, opcode);

    switch(opcode) 
    {
        // MISC
        case 0x00: 
            fprintf(debug_output, "NOP");
            break;
        case 0xF3:
            fprintf(debug_output, "DI");
            break;

        // XOR
        case 0xAF:
            fprintf(debug_output, "XOR A, A");
            break;

        // DEC
        case 0x05:
            fprintf(debug_output, "DEC B");
            break;
        case 0x0D:
            fprintf(debug_output, "DEC C");
            break;
        case 0x0B:
            fprintf(debug_output, "DEC BC");
            break;

        // CP
        case 0xFE:
            fprintf(debug_output, "CP A, n8");
            break;

        // LD
        case 0x3E:
            fprintf(debug_output, "LD A, n8");
            break;
        case 0x06:
            fprintf(debug_output, "LD B, n8");
            break;
        case 0x0E:
            fprintf(debug_output, "LD C, n8");
            break;
        case 0x21: 
            fprintf(debug_output, "LD HL, n16");
            break;
        case 0x32: 
            fprintf(debug_output, "LD [HL-], A");
            break;
        case 0xE0: 
            fprintf(debug_output, "LDH [a8], A");
            break;
        case 0xF0: 
            fprintf(debug_output, "LDH A, [a8]");
            break;
        case 0xEA:
            fprintf(debug_output, "LD [a16], A ");
            break;

        // JP
        case 0x20:
            fprintf(debug_output, "JR NZ e8");
            break;
        case 0xC3:
            fprintf(debug_output, "JP a16");
            break;
        

        default:
            fprintf(debug_output, "Unknown or unimplemented opcode\n");
            printf("Unknown Opcode\n");
            return false;
    }
    fprintf(debug_output, "\n");
    return true;
}
