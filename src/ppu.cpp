#include <MMU.hpp>


int MMU::PPU_render(uint32_t* pixels){
    int cycles = 0;

    uint8_t ly      = io[LY_REGISTER];
    uint8_t lyc     = io[LYC_REGISTER];
    uint8_t lcdc    = io[LCDC_REGISTER];
    uint8_t stat    = io[STAT_REGISTER];
    uint8_t scy     = io[SCY_REGISTER];
    uint8_t scx     = io[SCX_REGISTER];
    uint8_t bgp     = io[BGP_REGISTER];
    uint8_t obp0    = io[OBP0_REGISTER];
    uint8_t obp1    = io[OBP1_REGISTER];

    if(!(lcdc >> 7)) {
        write_io(0xFF41, (stat & 0xFE));
        
    }

    return cycles;
}
