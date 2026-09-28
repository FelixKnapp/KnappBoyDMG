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
    size_t debug_lines{0};

    uint8_t opcode;
    uint32_t address;

    bool prefixed{false};

    size_t cylces_intern_tima{0};
    size_t cylces_intern_div{0};
    bool enable_tima{false};

public:
    bool open_debug_file(std::string debug_output_path) {
        debug_output = fopen(debug_output_path.c_str(), "wb");
        if(!debug_output) {
            return false;
        }
        SDL_Log("Opened Debug File");
        fprintf(debug_output, "Started Emulator\n");
        return true;;
    }
    FILE* debug_output;
    registers_t reg;
    class MMU mmu;
    state_t state{RUNNING};



    // functions

    uint16_t emulate_inst();
    bool emulate_inst_debug();

    void update_timers(int cycles_passed);

    int handle_interrupt();
    
    
    // helper functions

    // fetch 8 bit integer immediate value, increments PC by 1 in the process
    uint8_t fetch_byte();
    // fetch 16 bit integer immediate value, increments PC by 2 in the process
    uint16_t fetch_word();
    // Push 8 bit integer value to stack, decrement SP by 1 in the process
    void push_byte(uint8_t value);
    // Push 16 bit integer value to stack, decrement SP by 2 in the process
    void push_word(uint16_t value);
    // set flag
    void set_flag(uint8_t flag, bool condition);
    // get flag
    bool get_flag(uint8_t flag);
    // Set all flags
    void set_all_flags(bool zero, bool subtract, bool half_carry, bool carry);
    // fetch clock select mode and set enable_tima
    uint8_t get_clock_select();
};
