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

// sets colour ids automatically
class Palette {
public:
    uint8_t value;
    uint8_t id_0;
    uint8_t id_1;
    uint8_t id_2;
    uint8_t id_3;

    Palette(uint8_t value) {
        this->value = value;
        id_0 = value & 0x3;
        id_1 = (value >> 2) & 0x3;
        id_2 = (value >> 4) & 0x3;
        id_3 = (value >> 6) & 0x3;
    }
};

void MMU::PPU_lcd_blank(uint32_t* pixels) {
    for(size_t i = 0; i < (WIDTH * HEIGHT); i++) {
        *pixels = PPU_colour;
        pixels++;
    }
}

void MMU::PPU_update(uint32_t* pixels, int cycles_passed){
    uint8_t ly          = io[LY_REGISTER];      // LCD Y Coordinate
    uint8_t lyc         = io[LYC_REGISTER];     // LY Compare
    uint8_t lcdc        = io[LCDC_REGISTER];    // LCD Control Byte
    uint8_t stat        = io[STAT_REGISTER];    // STAT Register
    uint8_t scy         = io[SCY_REGISTER];     // Background viewport Y
    uint8_t scx         = io[SCX_REGISTER];     // Background viewport X
    uint8_t bgp         = io[BGP_REGISTER];     // Background Palette
    uint8_t obp0        = io[OBP0_REGISTER];    // Object Palette 0
    uint8_t obp1        = io[OBP1_REGISTER];    // Object Palette 1
    //class Palette palette(io[0x47]);            // Colours mapped to colour ids

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


        if (ly < 144) {
            if(PPU_scanline_dot_counter < 80) {
                PPU_mode = OAM;
            } else if (PPU_scanline_dot_counter < 80 + 172) {
                PPU_mode = DRAW;
            } else {
                PPU_mode = HBLANK;
            }
        } 

        OAM_Sprite_t OAM_sprites[10];
        bool enable_bg = lcdc & 1;
        bool enable_window = enable_bg ? (lcdc >> 5) & 1: false;
        if(!enable_bg) lcdc = lcdc & ~(1 << 5);
        uint16_t bg_tilemap_area = (lcdc >> 3) & 1 ? 0x9C00 : 0x9800;
        uint16_t window_tilemap_area = (lcdc >> 6) & 1 ? 0x9C00 : 0x9800;
        


        // actual sequence were everything visible happens
        switch (PPU_mode)
        {
            case OAM: {
                uint8_t i = 0;
                uint8_t oam_index = 0;
                // fetch data from OAM
                while(i < 10 && oam_index < 0xA0) {
                    if((oam[oam_index] - 16 ) <= ly && (oam[oam_index] - 16 + height) > ly) {
                        OAM_sprites[i].y            = oam[oam_index];
                        OAM_sprites[i].x            = oam[oam_index + 1];
                        OAM_sprites[i].tile_index   = oam[oam_index + 2];
                        OAM_sprites[i].flags        = oam[oam_index + 3];
                        OAM_sprites[i].oam_index    = oam_index / 4;
                        i++;
                    }
                    oam_index+=4;
                }
                break;
            }


            case DRAW: {

                //* –––––––––Background drawing–––––––––
                uint8_t pos_y = (ly + scy) & 255;
                uint16_t tile_y = pos_y / 8;
                for(uint8_t x = 0; x < 160; x++) {
                    uint8_t pos_x = (x + scx) & 255;
                    uint16_t tile_x = pos_x / 8;
                    uint16_t tile_map_address = bg_tilemap_area + (tile_y * 32) + tile_x;
                    uint8_t tile_index = vram[tile_map_address - 0x8000];

                    uint16_t tile_address;
                    bool unsigned_mode = (lcdc >> 4) & 1;
                    uint8_t y_in_tile = pos_y % 8;
                    uint8_t x_in_tile = pos_x % 8;

                    if (unsigned_mode) {
                        // 0x8000 mode (unsigned)
                        tile_address = 0x8000 + (tile_index * 16) + (y_in_tile * 2);
                    } else {
                        // 0x8800 mode (signed)
                        tile_address = (uint16_t)(0x9000 + ((int8_t)tile_index * 16) + (y_in_tile * 2));
                    }
                        // fetch pixel data
                    uint8_t byte[2] = {vram[tile_address - 0x8000], vram[(tile_address + 1) - 0x8000]};

                    uint8_t bit_index = 7 - x_in_tile;
                    uint8_t low_bit = (byte[0] >> bit_index) & 1;
                    uint8_t high_bit = (byte[1] >> bit_index) & 1;
                    uint8_t color_id = (high_bit << 1) | low_bit;                    
                    


                    //* –––––––––window drawing–––––––––




                    //* –––––––––Sprite/Object Drawing–––––––––



                    // Palettes
                    /*uint8_t actual_color_id;
                    switch (color_id)
                    {
                        case 0b00:
                            actual_color_id = palette.id_0;
                            break;
                        case 0b01:
                            actual_color_id = palette.id_1;
                            break;
                        case 0b10:
                            actual_color_id = palette.id_2;
                            break;
                        case 0b11:
                            actual_color_id = palette.id_3;
                            break;
            
                        default: 
                            actual_color_id = 0;
                            break;
                    }*/

                    // TODO: proper palette implementation
                    // write to pointer
                    switch(color_id) {
                        case 0b00:
                            PPU_colour = LIGHTEST_GREEN;
                            break;
                        case 0b01:
                            PPU_colour = LIGHT_GREEN;
                            break;
                        case 0b10:
                            PPU_colour = DARK_GREEN;
                            break;
                        case 0b11:
                            PPU_colour = DARKEST_GREEN;
                            break;
                        default:
                            PPU_colour = WHITE;
                            break;
                    }
                    *(pixels + (ly * 160 + x)) = (uint32_t)PPU_colour;
                }
            }


            case VBLANK: {
                // nothing happens here in VBLANK
                break;
            }


            case HBLANK: {
                // Wait for end of Line
                break;
            }
        }

        if(PPU_scanline_dot_counter >= 456) {
            PPU_scanline_dot_counter -= 456;
            ly++;

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
