#include <iostream>
#include <fstream>
#include <json.hpp>

const std::string RESET_COLOUR = "\033[0m";
const std::string CYAN = "\033[1;36m";
const std::string GREEN = "\033[1;32m";
const std::string BLUE = "\033[1;34m";
const std::string MAGENTA = "\033[1;35m";

int main(int argc, char** argv) {

    if (argc != 4) {
        return 1;
    }
    std::string file_path   = argv[1];
    std::string prefix      = argv[2];
    std::string opcode      = argv[3];

    int found = opcode.find("0x");
    if(found == std::string::npos) {
        opcode.insert(0, "0x");
    }

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

    std::cout << GREEN << "Mnemonic: " << mnemonic << " ";

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


    std::cout << "  |  " << CYAN << "Lenght: " << bytes << RESET_COLOUR;

    // check if one or two cycle
    if (cycles_array.size() == 1) {
        std::cout << "  | " << BLUE <<  " Cycles: " << cycles_array[0];
    } else {
        std::cout << "  | " << BLUE <<  " Cycles: " << cycles_array[0] << "/" << cycles_array[1];
    }

    std::string flag_names[] = {
        "Z", "N", "H", "C"
    };

    std::cout << RESET_COLOUR << "  |  " << MAGENTA << "Flags: ";
    for(uint8_t i = 0; i < 4; i++) {
        std::string flag = json_data[prefix][opcode]["flags"][flag_names[i]];
        std::cout << flag << " ";
    }

    std::cout << std::endl;

    return 0;
}
