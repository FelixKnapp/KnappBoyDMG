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
    echo "-f start with rom"
    echo "-b build"
    echo "-g build debug"
    echo "-d output unimplemented opcodes from debug file"

    exit 0
}


while getopts 'dbghf:' FLAG; do
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
        g)
            build_debug_func
            ;;
        h)
            help_func
            ;;
        *)
            echo "Usage: gb <FLAGS> <ARGS>"
            echo "For help use -h"
            exit 1
            ;;
    esac
done

echo "Usage: gb <FLAGS> <ARGS>"
echo "For help use -h"
exit 1

exit 0
