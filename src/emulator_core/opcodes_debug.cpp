#include <gameboy.hpp>

// is not needed if all opcodes are implemented
bool Gameboy::emulate_inst_debug() {

    inst_nr++;
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
        case 0xF3:

        // OR
        case 0xB1:

        // XOR
        case 0xAE:
        case 0xAF:

        // DEC
        case 0x05:
        case 0x0D:
        case 0x0B:

        // CP
        case 0xFE:

        // INC
        case 0x03:
        case 0x04:
        case 0x0C:
        case 0x14:

        // ADC
        case 0x8C:

        // RL
        case 0x07:

        // LD
        case 0x01: 
        case 0x06:
        case 0x0E:
        case 0x12:
        case 0x1A:
        case 0x21: 
        case 0x26: 
        case 0x2A:
        case 0x31:
        case 0x32: 
        case 0x36:
        case 0x3E:
        case 0x44:
        case 0x78: 
        case 0x79: 
        case 0xE0: 
        case 0xF0: 
        case 0xE2:
        case 0xEA:

        // JP
        case 0x20:
        case 0xC3:

        break;

        // CALL
        case 0xCD: 
            //printf("Inst NR: %d, Address: 0x%04X, Opcode: 0x%02X, Desc: CALL\n", inst_nr, reg.PC, opcode);
            break;

        // RET
        case 0xC0:
            /*printf("Inst NR: %d, Address: 0x%04X, Opcode: 0x%02X, Desc: RET NZ - ", inst_nr, reg.PC, opcode);
            if(!get_flag(Flag::Z)) printf("Returned\n");
            else  printf("Not returned\n");
            break;*/
        case 0xC9:
            //printf("Inst NR: %d, Address: 0x%04X, Opcode: 0x%02X, Desc: RET\n", inst_nr, reg.PC, opcode);
            break;

        default:  
            printf("Inst NR: %d, Address: 0x%04X, Opcode: 0x%02X, Desc: ", inst_nr, address, opcode);
            printf("Unknown or unimplemented opcode\n");
            return false;
    }
    return true;
}

bool Gameboy::emulate_inst_prefix_debug(uint8_t opcode) {
    switch(opcode) {
        default:   
            printf("PREFIXED: Address: 0x%04X, Opcode: 0x%02X, Desc: ", address, opcode);
            printf("Unknown or Unimplemented Opcode\n");
            return false;
    }
    return true;
}