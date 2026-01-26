# Compiler and flags
CC      = gcc
CFLAGS  = -Wall -Wextra -O2

# Installation paths
PREFIX  = $(HOME)/.local
BINDIR  = $(PREFIX)/bin
DATADIR = $(PREFIX)/share/affirm

# Files
SRC     = src/main.c
BIN     = affirm

# Default target
all: $(BIN)

# Build the executable
$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

# Install binary and data files
install: $(BIN)
	mkdir -p $(BINDIR)
	mkdir -p $(DATADIR)
	chown -R $(USER):$(USER) $(PREFIX)
	cp $(BIN) $(BINDIR)/$(BIN)
	cp data/*.txt $(DATADIR)/


# Remove build artifacts
clean:
	rm -f $(BIN)

.PHONY: all install clean
