#include <gameboy.hpp>

void Gameboy::emulate_inst_prefix(uint8_t opcode) {
    switch(opcode) {
        default:
            SDL_Log("Unknown opcode: %d\n", opcode);
            return;
    }
}
