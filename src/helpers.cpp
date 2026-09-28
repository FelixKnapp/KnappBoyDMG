#include <gameboy.hpp>

uint8_t Gameboy::fetch_byte() {
    return mmu.read(reg.PC++);
}

uint16_t Gameboy::fetch_word() {
    uint8_t lsb = fetch_byte();
    uint16_t msb = static_cast<uint16_t>(fetch_byte());
    return ((msb << 8) | lsb);
}

void Gameboy::set_flag(uint8_t flag, bool condition) {
    if (condition) {
        reg.F |= flag;
    } else {
        reg.F &= ~flag;
    }
    return;
}

bool Gameboy::get_flag(uint8_t flag) {
    return (reg.F & flag) != 0;
}

void Gameboy::set_all_flags(bool zero, bool subtract, bool half_carry, bool carry) {
    set_flag(Flag::Z, zero);
    set_flag(Flag::N, subtract);
    set_flag(Flag::H, half_carry);
    set_flag(Flag::C, carry);
}


void Gameboy::push_byte(uint8_t value) {
    mmu.write(reg.SP--, value);
    return;
}

void Gameboy::push_word(uint16_t value) {
    uint8_t msb = value >> 8;
    uint8_t lsb = static_cast<uint8_t>(value & 0x0F);
    push_byte(msb);
    push_byte(lsb);
    return;
}