#!/bin/bash

EMU_PATH=/home/felix/Projects/Coding/KnappBoyDMG/
EMU_BIN=$EMU_PATH/bin/gb
DEBUG_FILE=$EMU_PATH/debug_dir/debug.txt

run_func(){
    FILE_NAME="$OPTARG"
    $EMU_BIN $FILE_NAME
    exit 0
}

build_func(){
    make
    exit 0
}

build_debug_func(){
    make -j8 debug
    exit 0
}

help_func(){
    echo "Help: "
    echo "Usage: gb <FLAGS> <ARGS>"
    echo "FLAGS:"
    echo "-h help"
    echo "-f <arg> start with rom"
    echo "-b build"
    echo "-D build debug"
    echo "-c clean"
    echo "-O build opcode reader"
    echo "-o <arg> use opcode reader unprefixed, for help use -o help"
    echo "-p <arg> use opcode reader prefixed, for help use -p help"
    echo "-t print opcode table arg = prefixed/unprefixed"
    echo "-T build opcode table"

    exit 0
}


while getopts 'bDhf: Oo: p: ct: T' FLAG; do
    case "$FLAG" in
        f)
            run_func
            ;;
        b)
            build_func
            ;;
        D)
            build_debug_func
            ;;
        h)
            help_func
            ;;
        O)
            make opcode_reader -j8
            exit 0
            ;;
        o)
            $EMU_PATH/bin/opcode_reader $EMU_PATH/docs/opcodes.json unprefixed $OPTARG
            exit 0
            ;;
        p)
            $EMU_PATH/bin/opcode_reader $EMU_PATH/docs/opcodes.json prefixed $OPTARG
            exit 0
            ;;
        c)
            make clean
            exit 0
            ;;
        t)
            $EMU_PATH/bin/opcode_table $EMU_PATH/docs/opcodes.json $OPTARG
            exit 0
            ;;
        T)
            make -j8 opcode_table
            exit 0
            ;;
        *)
            echo "Usage: gb <FLAGS> <ARGS>"
            echo "For help use -h"
            exit 1
            ;;
    esac
done

exit 0
