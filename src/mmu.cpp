#include <MMU.hpp>

size_t get_ram_size(uint8_t ram_type) {
    switch (ram_type) 
    {
        case 0x02: return 0x2000;
        case 0x03: return 0x8000;
        case 0x04: return 0x20000;
        case 0x05: return 0x10000;
    }
    return 0x00;
}

std::unique_ptr<Cartridge> create_cartridge(std::vector<uint8_t> rom) {
    uint8_t mbc_type = rom.at(0x147);
    uint8_t ram_type = rom.at(0x149);
    size_t ram_size = get_ram_size(ram_type);

    switch(mbc_type) 
    {
        case 0x00: // ROM only
            SDL_Log("Rom only mode activated.\n");
            return std::make_unique<ROM_ONLY>(std::move(rom));

        default:
            SDL_Log("Unimplemented MBC-Type: 0x%02X.\nStopping Emulator\n");
            return nullptr;
    }
}