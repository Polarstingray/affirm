#!/bin/bash
set -euo pipefail
echo "setting up affirm..."

JOB_DIR="${1:-$(cd "$(dirname "$0")" && pwd)}"
cd "$JOB_DIR"

echo "cleaning..."
make clean
echo "compiling..."
make
echo "creating directories and installing binary..."
make install

BIN_DIR="$HOME/.local/bin"
if ! echo "$PATH" | grep -q "$BIN_DIR"; then
    echo "adding $BIN_DIR to $HOME/.bashrc"
    echo "export PATH=\"$PATH:$BIN_DIR\"" >> "$HOME/.bashrc"
    export PATH="$PATH:$BIN_DIR"
fi
