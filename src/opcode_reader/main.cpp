#include <iostream>
#include <fstream>
#include <json.hpp>
#include <colours.hpp>

// TODO: Add Link to Output
int main(int argc, char** argv) {

    if (argc != 4) {
        return 1;
    }
    std::string file_path   = argv[1];
    std::string prefix      = argv[2];
    std::string opcode      = argv[3];

    int found = opcode.find("0x");
    std::stringstream opcode_stream;
    if(found == std::string::npos) {
        opcode_stream << "0x";
    }
    opcode_stream << std::uppercase << opcode;
    opcode = opcode_stream.str();

    if(prefix == "help") {
        std::cout << "Flag order: Z N H C" <<  std::endl;
        return 0;
    }

    std::ifstream json_file(file_path.c_str());

    if(!json_file.is_open()) {
        std::cerr << "Couldnt open json file!" << std::endl;
        return 1;
    }

    nlohmann::json json_data;
    json_file >> json_data;

    // fetch data
    std::string mnemonic = json_data[prefix][opcode]["mnemonic"];   // operation name
    int bytes = json_data[prefix][opcode]["bytes"];                 // operation length in bytes
    auto cycles_array = json_data[prefix][opcode]["cycles"];        // operation clock cycles in T-Cycles

    std::cout << YELLOW_TEXT << "Opcode: "   << opcode   << RESET_COLOUR << "  |  ";
    std::cout << GREEN_TEXT  << "Mnemonic: " << mnemonic << " ";

    // fetch operands
    bool first_run = true;
    for (const auto& operand : json_data[prefix][opcode]["operands"]) {
        bool immediate = operand["immediate"];
        std::string name = operand["name"];
        if(first_run) {
            if (!immediate) {
                std::cout << "[" << name;
                if(operand.contains("decrement")){
                    std::cout << "-";
                }
                if(operand.contains("increment")) {
                    std::cout << "+";
                }
                std::cout  << "]";
            } else {
                std::cout << name;
                if(operand.contains("decrement")){
                    std::cout << "-";
                }
                if(operand.contains("increment")) {
                    std::cout << "+";
                }
            }
            first_run = false;
        } else {
            if (!immediate) {
                std::cout << ", [" << name;
                if(operand.contains("decrement")){
                    std::cout << "-";
                }
                if(operand.contains("increment")) {
                    std::cout << "+";
                }
                std::cout << "]";
            } else {
                std::cout << ", " << name;
                if(operand.contains("decrement")){
                    std::cout << "-";
                }
                if(operand.contains("increment")) {
                    std::cout << "+";
                }
            }
        }

    }   
    std::cout << RESET_COLOUR;


    std::cout << "  |  " << CYAN_TEXT << "Lenght: " << bytes << RESET_COLOUR;

    // check if one or two cycle
    if (cycles_array.size() == 1) {
        std::cout << "  | " << BLUE_TEXT <<  " Cycles: " << cycles_array[0];
    } else {
        std::cout << "  | " << BLUE_TEXT <<  " Cycles: " << cycles_array[0] << "/" << cycles_array[1];
    }

    std::string flag_names[] = {
        "Z", "N", "H", "C"
    };

    std::cout << RESET_COLOUR << "  |  " << MAGENTA_TEXT << "Flags: ";
    for(uint8_t i = 0; i < 4; i++) {
        std::string flag = json_data[prefix][opcode]["flags"][flag_names[i]];
        std::cout << flag << " ";
    }

    std::cout << std::endl;

    return 0;
}
