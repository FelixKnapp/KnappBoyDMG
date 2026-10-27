#include <iostream>
#include <vector>

#include <gameboy.hpp>

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>

#define A_BUTTON_SDL        SDLK_O
#define B_BUTTON_SDL        SDLK_P
#define SELECT_BUTTON_SDL   SDLK_RETURN
#define START_BUTTON_SDL    SDLK_I

#define RIGHT_BUTTON_SDL    SDLK_D
#define LEFT_BUTTON_SDL     SDLK_A
#define UP_BUTTON_SDL       SDLK_W
#define DOWN_BUTTON_SDL     SDLK_S

#define SCREEN_WIDTH        160
#define SCREEN_HEIGHT       144

const double TARGET_FRAME_MS = 1000.0 / 59.73;


typedef struct{
    SDL_Window*         window;
    SDL_Renderer*       renderer;
    SDL_AudioSpec       want, have;
    SDL_Texture*        texture;
    SDL_AudioDeviceID   dev;
} sdl_t;

typedef struct{
    uint16_t window_width;
    uint16_t window_height;
    uint8_t scale_factor;
} config_t;

void set_configs(config_t* config) {
    config->window_width = 256;
    config->window_height = 224;
    std::cout << "Please Enter the scale Factor (Original Resolution: 160x144): ";
    scanf("%hhd", &config->scale_factor);
}

bool init_sdl(sdl_t* sdl, config_t config)
{
    if(!SDL_InitSubSystem(SDL_INIT_AUDIO)) {   
        SDL_Log("<ERROR> SDL Subsystem Audio could not be initialized: %s\n", SDL_GetError());
    }
    sdl->window = SDL_CreateWindow("GameBoy Emulator", config.window_width * config.scale_factor, config.window_height * config.scale_factor, 0);

    if(!sdl->window) {
        SDL_Log("<ERROR> SDL Window could not be initialized: %s\n", SDL_GetError());
        return false;
    }

    sdl->renderer = SDL_CreateRenderer(sdl->window, NULL);

    if(!sdl->renderer) {
        SDL_Log("<ERROR> SDL Renderer could not be initialized: %s\n", SDL_GetError());
        return false;
    }

    sdl->texture = SDL_CreateTexture (
        sdl->renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        160,
        144
    );
    SDL_SetTextureScaleMode(sdl->texture, SDL_SCALEMODE_NEAREST);

    if(!sdl->texture) {
        SDL_Log("<ERROR> SDL Texture could not be initialized: %s\n", SDL_GetError()); 
        return false;
    }

    return true;
}



bool init_gameboy(class Gameboy* gb, char* rom_name) {
    const uint32_t entry_point = 0x0000;
    FILE* rom_current = fopen(rom_name, "rb");
    if(rom_current == NULL) {
        SDL_Log("Could not open rom file\n");
        return false;
    }

    fseek(rom_current, 0 , SEEK_END);
    const size_t rom_size = ftell(rom_current);
    rewind(rom_current);

    if (rom_size < 0x0150) {
        SDL_Log("ROM file is too small or invalid\n");
        fclose(rom_current);
        return false;
    }

    std::vector<uint8_t> rom_buffer(rom_size);

    size_t read_bytes = fread(rom_buffer.data(), 1, rom_size, rom_current);
    fclose(rom_current);

    if (read_bytes < rom_size) {
        SDL_Log("Error while reading ROM\n");
        return false;
    }

    auto cartridge = create_cartridge(rom_buffer);

    if (!cartridge) {
        SDL_Log("Couldnt Create Cartridge\n");
        return false;
    }

    gb->mmu.load_cartridge(std::move(cartridge));

    gb->reg.PC = entry_point;
    gb->reg.SP = 0xFFFE;
    return true;
}

void clear_screen(sdl_t sdl) {
    SDL_SetRenderDrawColor(sdl.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(sdl.renderer);
    SDL_RenderPresent(sdl.renderer);
}

void handle_input(class Gameboy* gb) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) 
    {
        switch (event.type)            
        {
            case SDL_EVENT_QUIT:
                gb->state = STOPPED;
                break;

            case SDL_EVENT_KEY_DOWN: {
                bool is_pressed = true;
                switch (event.key.key) 
                {
                    case SDLK_SPACE:
                        if(gb->state == RUNNING) {
                            gb->state = PAUSED;
                            SDL_Log("==========PAUSED==========");
                        }
                        else {
                            gb->state = RUNNING;
                            SDL_Log("=========UNPAUSED=========");
                        }
                        break;

                    case SDLK_ESCAPE:
                        gb->state = STOPPED;
                        break;
                    
                    case RIGHT_BUTTON_SDL: gb->mmu.handle_input(0, false, is_pressed);  break;
                    case LEFT_BUTTON_SDL: gb->mmu.handle_input(1, false, is_pressed);   break;
                    case UP_BUTTON_SDL: gb->mmu.handle_input(2, false, is_pressed);     break;
                    case DOWN_BUTTON_SDL: gb->mmu.handle_input(3, false, is_pressed);   break;

                    case A_BUTTON_SDL: gb->mmu.handle_input(0, true, is_pressed);       break;
                    case B_BUTTON_SDL: gb->mmu.handle_input(1, true, is_pressed);       break;
                    case SELECT_BUTTON_SDL: gb->mmu.handle_input(2, true, is_pressed);  break;
                    case START_BUTTON_SDL: gb->mmu.handle_input(3, true, is_pressed);   break;
                }
                break;
            }
            case SDL_EVENT_KEY_UP: {
                bool is_pressed = false;
                switch (event.key.key) 
                {
                    case RIGHT_BUTTON_SDL: gb->mmu.handle_input(0, false, is_pressed);  break;
                    case LEFT_BUTTON_SDL: gb->mmu.handle_input(1, false, is_pressed);   break;
                    case UP_BUTTON_SDL: gb->mmu.handle_input(2, false, is_pressed);     break;
                    case DOWN_BUTTON_SDL: gb->mmu.handle_input(3, false, is_pressed);   break;

                    case A_BUTTON_SDL: gb->mmu.handle_input(0, true, is_pressed);       break;
                    case B_BUTTON_SDL: gb->mmu.handle_input(1, true, is_pressed);       break;
                    case SELECT_BUTTON_SDL: gb->mmu.handle_input(2, true, is_pressed);  break;
                    case START_BUTTON_SDL: gb->mmu.handle_input(3, true, is_pressed);   break;
                }
                break;
            }
        }
    }
}

void end_cleanup(sdl_t sdl) {
    SDL_DestroyRenderer(sdl.renderer);
    SDL_DestroyWindow(sdl.window);
    SDL_Quit();
}


int main(int argc, char** argv) {
    std::string debug_path = "debug_dir/debug.txt";
    sdl_t sdl;
    config_t config;
    class Gameboy gb;

    if(argc > 2) {
        SDL_Log("Too many arguments\n");
        exit(EXIT_FAILURE);
    } else if(argc < 2) {
        SDL_Log("Too little arguments\n");
        exit(EXIT_FAILURE);
    }

    set_configs(&config);

    if(!init_sdl(&sdl, config)) {
        SDL_Log("Couldnt initialize SDL, aborting\n");
        exit(EXIT_FAILURE);
    }

    if(!init_gameboy(&gb, argv[1])) {
        SDL_Log("Couldnt initialize Gameboy, aborting\n");
        end_cleanup(sdl);
        exit(EXIT_FAILURE);
    }

    clear_screen(sdl);

    uint32_t* pixels = (uint32_t*)malloc(sizeof(uint32_t) * SCREEN_HEIGHT * SCREEN_WIDTH);

    // main emulator loop
    while(gb.state != STOPPED) {
        handle_input(&gb);
        if(gb.state == PAUSED) continue;
        
        uint64_t start_frame_time = SDL_GetPerformanceCounter();

        for(uint8_t scanline = 0; scanline < 154; scanline++) {
            uint16_t scanline_cycles = 0;
            for(; scanline_cycles < 456;) {
                uint8_t inst_cycles = 0;
                inst_cycles += gb.handle_interrupt(); 
                gb.update_timers(inst_cycles);
                inst_cycles += gb.emulate_inst();
                #ifdef DEBUG
                if(!gb.emulate_inst_debug()) {
                    goto end_success;
                }
                #endif
                scanline_cycles += inst_cycles;
            }
            gb.mmu.PPU_update(pixels, scanline_cycles);
        }


        SDL_UpdateTexture(sdl.texture, NULL, pixels, 160 * sizeof(uint32_t));
        SDL_RenderClear(sdl.renderer);
        SDL_RenderTexture(sdl.renderer, sdl.texture, NULL, NULL);
        SDL_RenderPresent(sdl.renderer);

        uint64_t end_frame_time = SDL_GetPerformanceCounter();

        double elapsed_time = (double)((end_frame_time - start_frame_time) * 1000) / SDL_GetPerformanceFrequency();
        double waited_time = TARGET_FRAME_MS - elapsed_time;

        if (elapsed_time < TARGET_FRAME_MS) {
            SDL_Delay((uint32_t)(waited_time));
            std::cout << "Delayed: " << waited_time << "ms" <<std::endl;
        }
    }
    goto end_success;

    end_success:
    end_cleanup(sdl);

    return 0;
}
