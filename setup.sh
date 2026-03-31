#!/bin/sh

set -e

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
OUT_DIR="$PROJECT_DIR/out"

echo "Building project..."
make -C "$PROJECT_DIR" clean
make -C "$PROJECT_DIR"

echo "Creating output directory structure..."
rm -rf "$OUT_DIR"
mkdir -p "$OUT_DIR/bin"
mkdir -p "$OUT_DIR/data"

echo "Copying build artifacts..."
cp "$PROJECT_DIR/affirm" "$OUT_DIR/bin/"
cp "$PROJECT_DIR"/data/* "$OUT_DIR/data/"

chmod +x "$OUT_DIR/bin/affirm"

echo "Done!"
