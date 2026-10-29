#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <tabulate.hpp>
#include <json.hpp>
#include <colours.hpp>

// get Color for corresponding opcode
tabulate::Color get_opcode_color_unprefixed(std::string mnemonic) {
    tabulate::Color colour_cell;
  
    // Misc / Control Instructions (Red)
    if (mnemonic == "STOP"   || mnemonic == "HALT" ||
        mnemonic == "PREFIX" || mnemonic == "DI"   || mnemonic == "EI") {
        colour_cell = tabulate::Color::red;
    }
    // Jumps / Calls (Orange)
    else if (mnemonic == "JR"  || mnemonic == "JP" || mnemonic == "CALL" ||
             mnemonic == "RET" || mnemonic == "RST") {
        colour_cell = tabulate::Color::magenta;
    }
    // Load Instructions (Blue)
    else if (mnemonic == "LD") {
        colour_cell = tabulate::Color::blue;
    }
    // stack based instructions (green)
    else if (mnemonic == "PUSH" || mnemonic == "POP") {
        colour_cell = tabulate::Color::green;
    }

    // Arithmetic / Logical Instructions (Olive/Yellow-Brown)
    

    // 8-bit Shift, Rotate and Bit Instructions (Cyan)
    else if (mnemonic == "RLCA" || mnemonic == "RRCA" || 
             mnemonic == "RLA"  || mnemonic == "RRA"  ) {
        colour_cell = tabulate::Color::cyan;
    }

    // Default (Grey)
    else {
      colour_cell = tabulate::Color::none;
    }
    
    return colour_cell;
}

tabulate::Color get_opcode_color_prefixed() {
    // all prefixed opcodes are bit intructions(Cyan)
    return tabulate::Color::cyan;
}

// Print Complete overview over all prefixed or unprefixed opcodes in form of a table
int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Only needs 1 Argument\n" << std::endl;
        return 1;
    }


    std::string json_path = argv[1];
    std::string prefix = argv[2];

    if(prefix == "Legend") {

        return 0;
    }

    if ((prefix != "prefixed") && (prefix != "unprefixed")) {
        std::cerr << "Argument shall only be \"prefixed\" or \"unprefixed\" or \"Legend\" for a detailed Legend" << std::endl;
        return 2;
    }




    
    tabulate::Table opcode_table;
    std::ifstream json_file(json_path.c_str());

    if(!json_file.is_open()) {
        std::cerr << "Couldnt open json file!" << std::endl;
        return 1;
    }

    nlohmann::json json_data;
    json_file >> json_data;


    opcode_table.use_unicode_borders(true, tabulate::BorderStyle::Heavy);

    tabulate::Table::Row_t headers = {
        "Opcodes", "0x0", "0x1", "0x2", "0x3", "0x4", "0x5", "0x6", 
        "0x7", "0x8", "0x9", "0xA", "0xB", "0xC", "0xD", "0xE", "0xF"
    };

    opcode_table.add_row(headers);
    
    for(int opcode_second_half = 0; opcode_second_half < 16; opcode_second_half++) {
        std::stringstream second_half_stream;

        second_half_stream << "0x" << std::hex << std::uppercase << opcode_second_half;
        std::string current_opcode_seconf_half = second_half_stream.str();

        tabulate::Table::Row_t row = {current_opcode_seconf_half};
        for(int opcode_first_half = 0; opcode_first_half < 16; opcode_first_half++){
            int opcode = (opcode_first_half << 4) | (opcode_second_half); 
            std::stringstream opcode_stream;
            opcode_stream << "0x" << std::hex << std::uppercase << opcode_second_half << opcode_first_half;
            std::string opcode_string = opcode_stream.str();
            std::string mnemonic;// = CYAN_BG;
            mnemonic += json_data[prefix][opcode_string]["mnemonic"];
            mnemonic += " ";

            // fetch operands
            bool first_run = true;
            for (const auto& operand : json_data[prefix][opcode_string]["operands"]) {
                bool immediate = operand["immediate"];
                std::string name = operand["name"];
                if(first_run) {
                    if (!immediate) {
                        mnemonic += "[" + name;
                        if(operand.contains("decrement")){
                            mnemonic += "-";
                        }
                        if(operand.contains("increment")) {
                            mnemonic += "+";
                        }
                        mnemonic += "]";
                    } else {
                        mnemonic += name;
                        if(operand.contains("decrement")){
                            mnemonic += "-";
                        }
                        if(operand.contains("increment")) {
                            mnemonic += "+";
                        }
                    }
                    first_run = false;
                } else {
                    if (!immediate) {
                        mnemonic += ", [" + name;
                        if(operand.contains("decrement")){
                            mnemonic += "-";
                        }
                        if(operand.contains("increment")) {
                            mnemonic += "+";
                        }
                        mnemonic += "]";
                    } else {
                        mnemonic += ", " + name;
                        if(operand.contains("decrement")){
                            mnemonic += "-";
                        }
                        if(operand.contains("increment")) {
                            mnemonic += "+";
                        }
                    }
                }
            
            }
            row.insert(row.end(), mnemonic);
        }
        
        opcode_table.add_row(row);
        for(int opcode_first_half = 0; opcode_first_half < 16; opcode_first_half++) {
            std::stringstream opcode_stream;
            opcode_stream << "0x" << std::hex << std::uppercase << opcode_second_half << opcode_first_half;
            std::string opcode_string = opcode_stream.str();
            std::string mnemonic = json_data[prefix][opcode_string]["mnemonic"];

            if(prefix == "unprefixed") {
                opcode_table[opcode_second_half + 1][opcode_first_half + 1].format()
                    .background_color(get_opcode_color_unprefixed(mnemonic))
                    .border_background_color(tabulate::Color::none)
                    .corner_background_color(tabulate::Color::none);
            } else {
                opcode_table[opcode_second_half + 1][opcode_first_half + 1].format()
                    .background_color(get_opcode_color_prefixed())
                    .corner_background_color(tabulate::Color::none)
                    .border_background_color(tabulate::Color::none);
            }
        }
    }
    

    std::cout << opcode_table << std::endl;
}
