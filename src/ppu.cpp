#include <MMU.hpp>
#define WIDTH  160
#define HEIGHT 144

// store sprite data from OAM in struct
typedef struct {
    uint8_t y;
    uint8_t x;
    uint8_t flags;
    uint8_t tile_index;
    uint8_t oam_index;
} OAM_Sprite_t;

void MMU::PPU_lcd_blank(uint32_t* pixels) {
    for(size_t i = 0; i < (WIDTH * HEIGHT); i++) {
        *pixels = PPU_colour;
        pixels++;
    }
}

void MMU::PPU_update(uint32_t* pixels, int cycles_passed){
    uint8_t ly      = io[LY_REGISTER];      // LCD Y Coordinate
    uint8_t lyc     = io[LYC_REGISTER];     // LY Compare
    uint8_t lcdc    = io[LCDC_REGISTER];    // LCD Control Byte
    uint8_t stat    = io[STAT_REGISTER];    // STAT Register
    uint8_t scy     = io[SCY_REGISTER];     // Background viewport Y
    uint8_t scx     = io[SCX_REGISTER];     // Background viewport X
    uint8_t bgp     = io[BGP_REGISTER];     // Background Palette
    uint8_t obp0    = io[OBP0_REGISTER];    // Object Palette 0
    uint8_t obp1    = io[OBP1_REGISTER];    // Object Palette 1

    if (ly == lyc) {
        write_io(0xFF41, (stat | (1 << 2)));
        write_io(0xFF0F, read(0xFF0F) | (1 << 1));
    }

    PPU_mode = static_cast<PPU_mode_t>(stat & 0xFC);

    if(!(lcdc >> 7)) {
        PPU_colour = WHITE;
        write_io(0xFF41, (stat & 0xFC));
        write_io(0xFF44, 0);
        PPU_lcd_blank(pixels);
        return;
    }

    // fetch obj height
    uint8_t height = (lcdc >> 2 ) & 1 ? 16 : 8;

    while(cycles_passed > 0) {
        PPU_scanline_dot_counter++; 
        cycles_passed--;
        printf("%d, ", PPU_scanline_dot_counter);

        if(PPU_scanline_dot_counter >= 456) {
            PPU_scanline_dot_counter -= 456;
            ly++;
            printf(" ly = %d\n", ly);

            if (ly == 144) {
                // set mode to VBLANK and request VBLANK  interrupt
                PPU_mode = VBLANK;
                write_io(0xFF0F, read(0xFF0F) | 1);
            } else if (ly > 153){
                // frame is over
                PPU_mode = OAM;
                ly = 0;
            }
        }        

        if (ly > 144) {
            if(PPU_scanline_dot_counter < 80) {
                PPU_mode = OAM;
            } else if (PPU_scanline_dot_counter < 80 + 172) {
                PPU_mode = DRAW;
            } else {
                PPU_mode = HBLANK;
            }
        } 

        // actual sequence were everything visible happens
        OAM_Sprite_t OAM_sprites[10];
        switch (PPU_mode)
        {
            case OAM: {
                uint8_t i = 0;
                uint8_t oam_index = 0;
                // fetch data from OAM
                while(i < 10 && oam_index < 0xA0) {
                    if((oam[oam_index] - 16 >= ly) && (oam[oam_index] - 16 + height < ly)) {
                        OAM_sprites[i].y            = oam[oam_index];
                        OAM_sprites[i].x            = oam[oam_index + 1];
                        OAM_sprites[i].tile_index   = oam[oam_index + 2];
                        OAM_sprites[i].flags        = oam[oam_index + 3];
                        OAM_sprites[i].oam_index    = oam_index / 4;
                        i++;
                    }
                    oam_index+=4;
                }
            }
            case DRAW: {

            }
            case VBLANK: {

            }
            default: {

            }
        }
    }

    int mode_intern;
    switch (PPU_mode)
    {
        case OAM:
            mode_intern = 2;
            break;
        case DRAW:
            mode_intern = 3;
            break;
        case VBLANK:
            mode_intern = 1;
            break;
        default:
            mode_intern = 0;
            break;
    }

    write_io(0xFF41, (stat & 0xFC) | mode_intern);

    io[LY_REGISTER] = ly;    
    io[LYC_REGISTER] = lyc;  
    io[LCDC_REGISTER] = lcdc;  
    io[STAT_REGISTER] = stat; 
    io[SCY_REGISTER] = scy;  
    io[SCX_REGISTER] = scx;  
    io[BGP_REGISTER] = bgp;
    io[OBP0_REGISTER] = obp0; 
    io[OBP1_REGISTER] = obp1;

    return;
}
