CXX=ccache g++
CXX_FLAGS=-O3 -march=native -fno-exceptions -fno-rtti -std=c++17

CPP_CORE_SRC=$(wildcard src/emulator_core/*.cpp)
C_CORE_SRC=$(wildcard src/emulator_core/*.c)

CPP_OPCODE_READER_SRC=$(wildcard src/opcode_reader/*.cpp)
C_OPCODE_READER_SRC=$(wildcard src/opcode_reader/*.c)

CPP_OPCODE_TABLE_SRC=$(wildcard src/opcode_table/*.cpp)
C_OPCODE_TABLE_SRC=$(wildcard src/opcode_table/*.c)

ROMS=$(wildcard roms/*.COM)
INCLUDE_PATH_CORE=headers/emulator_core
INCLUDE_PATH_OPCODE_READER=headers/opcode_reader
INCLUDE_PATH_OPCODE_TABLE=headers/opcode_table


.PHONY=all debug opcode_reader opcode_table clean

all:
	mkdir -p bin
	$(CXX) -o bin/gb -I $(INCLUDE_PATH_CORE) $(CPP_CORE_SRC) $(CXX_FLAGS) `pkg-config --cflags --libs sdl3`

debug:
	mkdir -p bin
	$(CXX) -o bin/gb -I $(INCLUDE_PATH_CORE) $(CPP_CORE_SRC) $(CXX_FLAGS) -DDEBUG `pkg-config --cflags --libs sdl3`

opcode_reader:
	mkdir -p bin
	$(CXX) -o bin/opcode_reader -I $(INCLUDE_PATH_OPCODE_READER) $(CPP_OPCODE_READER_SRC)

opcode_table:
	mkdir -p bin
	$(CXX) -o bin/opcode_table -I $(INCLUDE_PATH_OPCODE_TABLE) $(CPP_OPCODE_TABLE_SRC)

clean:
	rm -rf bin
	@echo "cleaned" 
