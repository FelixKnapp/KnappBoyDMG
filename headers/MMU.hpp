#pragma once

#include <vector>
#include <cstdint>
#include <memory>
#include <fstream>
#include <filesystem>
#include <string>
#include <SDL3/SDL.h>

class Cartridge {
public:
    virtual ~Cartridge() = default;
    virtual uint8_t read(uint16_t address) = 0;
    virtual void write(uint16_t address, uint8_t value) = 0;
};

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

class ROM_ONLY : public Cartridge {
std::vector<uint8_t> rom;
public:
    explicit ROM_ONLY(std::vector<uint8_t> rom_data) : rom(std::move(rom_data)) {};

    uint8_t read(uint16_t address) override {
        if ((address < 0x8000) && (address < rom.size())) {
            return rom.at(address);
        } 
        return 0xFF;
    }

    void write(uint16_t address, uint8_t value) override {
        
    }
};

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

class MBC1 : public Cartridge {

};

class MBC2 : public Cartridge {

};

class MBC3 : public Cartridge {

};

class MBC5 : public Cartridge {

};

class MBC6 : public Cartridge {

};

class MMU {
    uint8_t vram[0x2000]{};   // 8 KB Video RAM
    uint8_t wram[0x2000]{};   // 8 KB Work RAM
    uint8_t oam[0xA0]{};      // 160 Bytes Sprite-Attribute
    uint8_t io[0x80]{};       // I/O-Register
    uint8_t hram[0x7F]{};     // High RAM
    uint8_t ie_register{0};

    std::unique_ptr<Cartridge> cartridge;
    
public:
    MMU() {
        io[0x05] = 0x00; // TIMA
        io[0x06] = 0x00; // TMA
        io[0x07] = 0x00; // TAC
        io[0x10] = 0x80; // NR10
        io[0x11] = 0xBF; // NR11
        io[0x12] = 0xF3; // NR12
        io[0x14] = 0xBF; // NR14
        io[0x40] = 0x91; // LCDC
        io[0x41] = 0x85; // STAT 
        io[0x47] = 0xFC; // BGP 
        io[0x48] = 0xFF; // OBP0
        io[0x49] = 0xFF; // OBP1
    }

    void load_cartridge(std::unique_ptr<Cartridge> cart) {
        cartridge = std::move(cart);
    }

    Cartridge* get_cartridge() const {
        return cartridge.get();
    }

    uint8_t read(uint16_t address) {
        if (address < 0x8000 || (address >= 0xA000 && address < 0xC000)) {
            if(cartridge) return cartridge->read(address);
            return 0xFF;
        }
         else if (address < 0xA000) {
        return vram[address & 0x1FFF];
        } 
        else if (address < 0xE000) {
            return wram[address - 0xC000];
        } 
        else if (address < 0xFE00) {
            return wram[address - 0xE000]; 
        } 
        else if (address < 0xFEA0) {
            return oam[address - 0xFE00];
        } 
        else if (address < 0xFF00) {
            return 0xFF; // Unusable
        }
        else if (address == 0xFF0F) {
            return io[0x0F] | 0xE0; // IME (interrupt flag)
        }
        else if (address == 0xFF41) {
            return io[0x41] | 0x80;
        }
        else if (address < 0xFF80) {
            return io[address - 0xFF00];
        } 
        else if (address < 0xFFFF) {
            return hram[address - 0xFF80];
        } 
        else if (address == 0xFFFF) { // 0xFFFF
            return ie_register;
        }
        return 0x00;
    }

    void write(uint16_t address, uint8_t value) {
        if (address < 0x8000 || (address >= 0xA000 && address < 0xC000)) {
            if (cartridge) cartridge->write(address, value);
        } 
        else if (address < 0xA000) {
            vram[address & 0x1FFF] = value; // Verwende Maskierung
        } 
        else if (address < 0xE000) {
            wram[address - 0xC000] = value;
        } 
        else if (address < 0xFE00) {
            wram[address - 0xE000] = value; // Echo RAM
        } 
        else if (address < 0xFEA0) {
            oam[address - 0xFE00] = value;
        } 
        else if (address < 0xFF00) {
            // Unusable
        } 
        else if (address >= 0xFF04 && address <= 0xFF07) {
            io[address - 0xFF00] = value;
        }
        else if (address == 0xFF0F) {
            io[0x0F] = value;
        }
        else if (address < 0xFFFF) {
            hram[address - 0xFF80] = value;
        } 
        else if (address == 0xFFFF){ // 0xFFFF
            ie_register = value;
        }
        return;
    }
};