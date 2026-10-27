#include <gameboy.hpp>

// is not needed if all opcodes are implemented
bool Gameboy::emulate_inst_debug() {

    switch(opcode) 
    {
        // PREFIX
        case 0xCB:
            if(!emulate_inst_prefix_debug(prefixed_opcode)) {
                return false;
            }
            break;

        // MISC
        case 0x00:
            break;
        case 0xF3:
            break;

        // XOR
        case 0xAF:
            break;

        // DEC
        case 0x05:
            break;
        case 0x0D:
            break;
        case 0x0B:
            break;

        // CP
        case 0xFE:
            break;

        // LD
        case 0x3E:
            break;
        case 0x06:
            break;
        case 0x0E:
            break;
        case 0x01: 
            break;
        case 0x21: 
            break;
        case 0x2A:
            break;
        case 0x31:
            break;
        case 0x32: 
            break;
        case 0x36: 
            break;
        case 0xE0: 
            break;
        case 0xF0: 
            break;
        case 0xE2: 
            break;
        case 0xEA:
            break;

        // JP
        case 0x20:
            break;
        case 0xC3:
            break;

        // CALL
        case 0xCD:
            break;

        default:  
            printf("Address: %04X, Opcode: 0x%02X, Desc: ", reg.PC, opcode);
            printf("Unknown or unimplemented opcode\n");
            return false;
    }
    return true;
}

bool Gameboy::emulate_inst_prefix_debug(uint8_t opcode) {
    switch(opcode) {
        default:   
            printf("Address: %04X, Opcode: 0x%02X, Desc: ", reg.PC, opcode);
            printf("Unknown or Unimplemented Opcode\n");
            return false;
    }
    return true;
}