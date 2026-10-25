CPP_CORE_SRC=$(wildcard src/emulator_core/*.cpp)
C_CORE_SRC=$(wildcard src/emulator_core/*.c)

CPP_OPCODE_READER_SRC=$(wildcard src/opcode_reader/*.cpp)
C_OPCODE_READER_SRC=$(wildcard src/opcode_reader/*.c)

ROMS=$(wildcard roms/*.COM)
INCLUDE_PATH_CORE=headers/emulator_core
INCLUDE_PATH_OPCODE_READER=headers/opcode_reader


.PHONY=all debug opcode_reader clean

all:
	mkdir -p bin
	g++ -o bin/gb -I $(INCLUDE_PATH_CORE) $(CPP_CORE_SRC) `pkg-config --cflags --libs sdl3`

debug:
	mkdir -p bin
	mkdir -p debug_dir
	g++ -o bin/gb -I $(INCLUDE_PATH_CORE) $(CPP_CORE_SRC) -DDEBUG `pkg-config --cflags --libs sdl3`

opcode_reader:
	mkdir -p bin
	g++ -o bin/opcode_reader -I $(INCLUDE_PATH_OPCODE_READER) $(CPP_OPCODE_READER_SRC)

clean:
	rm -rf bin
	rm -rf debug_dir
	@echo "cleaned" 
