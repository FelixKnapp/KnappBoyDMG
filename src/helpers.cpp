#include <gameboy.hpp>

uint8_t Gameboy::fetch_byte() {
    return mmu.read(reg.PC++);
}

uint16_t Gameboy::fetch_word() {
    uint8_t lsb = fetch_byte();
    uint16_t msb = static_cast<uint16_t>(fetch_byte());
    return ((msb << 8) | lsb);
}
