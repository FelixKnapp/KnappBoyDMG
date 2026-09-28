#include <gameboy.hpp>

uint8_t Gameboy::fetch_byte() {
    return mmu.read(reg.PC++);
}

uint16_t Gameboy::fetch_word() {
    uint8_t lsb = fetch_byte();
    uint16_t msb = static_cast<uint16_t>(fetch_byte());
    return ((msb << 8) | lsb);
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