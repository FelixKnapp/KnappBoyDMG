#include <gameboy.hpp>

bool Gameboy::emulate_inst_debug() {

    #ifdef LOGGING
    fprintf(debug_output, "Inst NR: %05d, Address: 0x%04X, Opcode: 0x%02X, Desc: ", debug_lines++, address, opcode);
    #endif

    switch(opcode) 
    {
        // MISC
        case 0x00:
            #ifdef LOGGING    
            fprintf(debug_output, "NOP");
            #endif
            break;
        case 0xF3:
            #ifdef LOGGING    
            fprintf(debug_output, "DI");
            #endif
            break;


        // PREFIX
        case 0xCB:
        fprintf(debug_output, "Address: %04X, Opcode: %02X, Desc: ", reg.PC, opcode);
        fprintf(debug_output, "PREFIXED: ");
        if(!emulate_inst_prefix_debug(prefixed_opcode)) {
                fprintf(debug_output, "\n");
                return false;
            }
            break;



        // XOR
        case 0xAF:
            #ifdef LOGGING    
            fprintf(debug_output, "XOR A, A");
            #endif
            break;

        // DEC
        case 0x05:
            #ifdef LOGGING    
            fprintf(debug_output, "DEC B");
            #endif
            break;
        case 0x0D:
            #ifdef LOGGING    
            fprintf(debug_output, "DEC C");
            #endif
            break;
        case 0x0B:
            #ifdef LOGGING    
            fprintf(debug_output, "DEC BC");
            #endif
            break;

        // CP
        case 0xFE:
            #ifdef LOGGING    
            fprintf(debug_output, "CP A, n8");
            #endif
            break;

        // LD
        case 0x3E:
            #ifdef LOGGING    
            fprintf(debug_output, "LD A, n8");
            #endif
            break;
        case 0x06:
            #ifdef LOGGING    
            fprintf(debug_output, "LD B, n8");
            #endif
            break;
        case 0x0E:
            #ifdef LOGGING    
            fprintf(debug_output, "LD C, n8");
            #endif
            break;
        case 0x21: 
            #ifdef LOGGING    
            fprintf(debug_output, "LD HL, n16");
            #endif
            break;
        case 0x2A:
            #ifdef LOGGING
            fprintf(debug_output, "LD A, [HL+]");
            #endif
        case 0x31:
            #ifdef LOGGING    
            fprintf(debug_output, "LD SP, n16");
            #endif
            break;
        case 0x32: 
            #ifdef LOGGING    
            fprintf(debug_output, "LD [HL-], A");
            #endif
            break;
        case 0x36: 
            #ifdef LOGGING    
            fprintf(debug_output, "LD HL, n8");
            #endif
            break;
        case 0xE0: 
            #ifdef LOGGING    
            fprintf(debug_output, "LDH [a8], A");
            #endif
            break;
        case 0xF0: 
            #ifdef LOGGING    
            fprintf(debug_output, "LDH A, [a8]");
            #endif
            break;
        case 0xE2: 
            #ifdef LOGGING    
            fprintf(debug_output, "LDH [C], A");
            #endif
            break;
        case 0xEA:
            #ifdef LOGGING    
            fprintf(debug_output, "LD [a16], A ");
            #endif
            break;

        // JP
        case 0x20:
            #ifdef LOGGING    
            fprintf(debug_output, "JR NZ e8");
            #endif
            break;
        case 0xC3:
            #ifdef LOGGING    
            fprintf(debug_output, "JP a16");
            #endif
            break;

        default:  
            fprintf(debug_output, "Address: %04X, Opcode: 0x%02X, Desc: ", reg.PC, opcode);
            fprintf(debug_output, "Unknown or unimplemented opcode\n");
            printf("Unknown Opcode\n");
            return false;
    }
    #ifdef LOGGING
    fprintf(debug_output, "\n");
    #endif
    return true;
}

bool Gameboy::emulate_inst_prefix_debug(uint8_t opcode) {
    switch(opcode) {
        default:   
            fprintf(debug_output, "Unknown or Unimplemented Opcode");
        return false;
    }
    return true;
}