#include <gameboy.hpp>

uint16_t Gameboy::emulate_inst() {
    opcode = mmu.read(reg.PC);
    address = reg.PC;
    reg.PC++;

    switch(opcode) {
        
        //* ____MISC____
        case 0x00: 
            // NOP          Cycles: 4       Length: 1
            return 4;

        case 0xF3:
            // DI           Cycles: 4       Length: 1
            reg.IME = false;
            return 4;
        case 0xCB:
            // PREFIX       Cycles: 4 + 4   Length: 1 + 1
            prefixed_opcode = mmu.read(reg.PC++);
            emulate_inst_prefix(prefixed_opcode);
            return 8;


        //* ____XOR____
        case 0xAF:
            // XOR A,A      Cycles: 4       Length: 1       Flags: 1 0 0 0
            reg.A ^= reg.A;
            set_all_flags(true, false, false, false);
            return 4;


        //* ___DEC____
        case 0x05: {
        // DEC B            Cycles: 4       Length: 1       Flags: Z 1 H -
            bool half_carry = (reg.B & 0xF) == 0;
            reg.B--;
            set_flag(Flag::Z, reg.B == 0);
            set_flag(Flag::N, true);
            set_flag(Flag::H, half_carry);
            return 4;
        }
        case 0x0D: {
            // DEC C        Cycles: 4       Length: 1       Flags: Z 1 H -
            bool half_carry = (reg.C & 0xF) == 0;
            reg.C--;
            set_flag(Flag::Z, reg.C == 0);
            set_flag(Flag::N, true);
            set_flag(Flag::H, half_carry);
            return 4;
        }

        case 0x0B:
            // DEC BC       Cycles: 8       Length: 1
            reg.BC--;
            return 8;


        //* ____CP____
        case 0xFE: {
            // CP A, n8     Cycles: 8       Length: 2       Flags: Z 1 H C
            uint8_t n8 = fetch_byte();
            uint8_t result = reg.A - n8;
            bool zero = (result == 0);
            bool half_carry = (reg.A & 0xF) < (n8 & 0xF);
            bool carry = reg.A < n8;
            set_all_flags(zero, true, half_carry, carry);
            return 8;
        }


        //* ____LD____
        case 0x3E:
            // LD A, n8     Cycles: 8       Length: 2
            reg.A = fetch_byte();
            return 8;
        case 0x06:
            // LD B, n8     Cycles: 8       Length: 2
            reg.B = fetch_byte();
            return 8;
        case 0x0E:
            // LD C, n8     Cycles: 8       Length: 2
            reg.C = fetch_byte();
            return 8;

        case 0x21: 
            // LD HL, n16   Cycles: 12      Length: 3
            reg.HL = fetch_word();
            return 12;
        case 0x31:
            // Mnemonic: LD SP, n16  |  Lenght: 3  |  Cycles: 12  |  Flags: - - - - 
            reg.SP = fetch_word();
            return 12;

        case 0x32:
            // LD [HL-], A  Cycles: 8       Length: 1
            mmu.write(reg.HL--, reg.A);
            return 8;
        case 0x2A: 
            //  LD A, [HL+]  |  Lenght: 1  |  Cycles: 8  |  Flags: - - - - 
            reg.A = mmu.read(reg.HL++);
            return 8;
        case 0x36: {
            // LD [HL] , n8 |  Lenght: 2    Cycles: 12    Flags: - - - - 
            uint8_t n8 = fetch_byte();
            mmu.write(reg.HL, n8);
            return 12;
        }
        
        case 0xE0: {
            // LDH [a8], A  Cycles: 12      Length: 2
            uint8_t a8 = fetch_byte();
            mmu.write(0xFF00 + a8, reg.A);
            return 12;
        }
        case 0xF0: {
            // LDH A, [a8]  Cycles: 12      Length: 2
            uint8_t a8 = fetch_byte();
            reg.A = mmu.read(0xFF00 + a8);
            return 12;
        }
        case 0xE2: {
            // LDH [C], A  |  Lenght: 1  |  Cycles: 8  |  Flags: - - - - 
            mmu.write(0xFF00 + reg.C, reg.A);
        }
        case 0xEA: {
            // LD [a16], A  Cycles: 16      Length: 3
            uint16_t a16 = fetch_word();
            mmu.write(a16, reg.A);
            return 16;
        }
        

        //* ____JP____
        case 0x20: {
            // JR NZ, e8    Cycles: 12/8    Length: 2
            int8_t e8 = static_cast<int8_t>(fetch_byte());
            if(!get_flag(Flag::Z)) {
                reg.PC += e8;
                return 12;
            }
            return 8;
        }
        case 0xC3: {
            // JP a16       Cycles: 16      Length: 3
            uint16_t a16 = fetch_word();
            reg.PC = a16;
            return 16;
        }

        default:
                return 0;
    }
}