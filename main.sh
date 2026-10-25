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
    make debug
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
    echo "-d output unimplemented opcodes from debug file"
    echo "-O build opcode reader"
    echo "-o <arg> use opcode reader unprefixed"
    echo "-p <arg> use opcode reader prefixed"

    exit 0
}


while getopts 'dbghf: Oo: p: ' FLAG; do
    case "$FLAG" in
        f)
            run_func
            ;;
        d)
            echo "Unimplemented Opcodes: "
            cat $DEBUG_FILE | grep "Unknown"
            exit 0
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
            make opcode_reader
            ;;
        o)
            $EMU_PATH/bin/opcode_reader $EMU_PATH/docs/opcodes.json unprefixed $OPTARG
            exit 0
            ;;
        p)
            $EMU_PATH/bin/opcode_reader $EMU_PATH/docs/opcodes.json cbprefixed $OPTARG
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
