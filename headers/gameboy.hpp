#pragma once
#include <registers.hpp>
#include <cstdint>
#include <MMU.hpp>

typedef enum {
    RUNNING = 0,
    PAUSED  = 1,
    STOPPED = 2
} state_t;

namespace Flag {
    constexpr uint8_t Z = 1 << 7;
    constexpr uint8_t N = 1 << 6;
    constexpr uint8_t H = 1 << 5;
    constexpr uint8_t C = 1 << 4;
}

class Gameboy {

public:
    class MMU mmu;
    state_t state{RUNNING};
    registers_t reg;
};