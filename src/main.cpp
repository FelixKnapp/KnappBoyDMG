#include <iostream>
#include <vector>

#include <gameboy.hpp>

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>

typedef struct{
    SDL_Window*         window;
    SDL_Renderer*       renderer;
    SDL_AudioSpec       want, have;
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
    std::cout << "Please Enter the scale Factor (Original Resolution: 256x224): ";
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

    return true;
}



bool init_gameboy(class Gameboy* gb, char* rom_name) {
    const uint32_t entry_point = 0x0100;
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
        end_cleanup(sdl);
        exit(EXIT_FAILURE);
    }

    if(!init_gameboy(&gb, argv[1])) {
        SDL_Log("Couldnt initialize Gameboy, aborting\n");
        end_cleanup(sdl);
        exit(EXIT_FAILURE);
    }


    clear_screen(sdl);
    // main emulator loop
    while(gb.state != STOPPED) {
        handle_input(&gb);
        if(gb.state == PAUSED) continue;

    }

    return 0;
}
