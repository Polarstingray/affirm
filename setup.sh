#!/bin/bash

echo "Compiling and Installing affirm..."
curr=$(pwd)

in_affirm=$(echo $curr | grep "affirm")
if [ $? -ne 0 ]; then
    cd "$curr/affirm"
    if [ $? -ne 0 ]; then
        echo "unable to find affirm directory"
        echo "Try running the setup.sh in the /affirm directory"
        exit 1
    fi
fi

make clean
make
make install
BIN_DIR="$HOME/.local/bin"
in_path=$(echo $PATH | grep "$BIN_DIR")
if [ $? -ne 0 ]; then
    echo "adding $BIN_DIR to $HOME/.bashrc"
    echo "export PATH=$PATH:$BIN_DIR" >> $HOME/.bashrc
fi

cd $curr
