CPP_SRC=$(wildcard src/*.cpp)
C_SRC=$(wildcard src/*.c)
ROMS=$(wildcard roms/*.COM)
INCLUDE_PATH=headers


.PHONY=all debug clean

all:
	mkdir -p bin
	g++ -o bin/gb -I $(INCLUDE_PATH) $(CPP_SRC) `pkg-config --cflags --libs sdl3`

debug:
	mkdir -p bin
	mkdir -p debug_dir
	g++ -o bin/gb -I $(INCLUDE_PATH) $(CPP_SRC) -DDEBUG `pkg-config --cflags --libs sdl3`

clean:
	rm -rf bin
	rm -rf debug_dir
	@echo "cleaned" 
