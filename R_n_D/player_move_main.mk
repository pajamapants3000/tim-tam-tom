#
# player_move_main.mk
# Makefile for player_move_main
#
# Compiler Flags
CC = gcc
CFLAGS = -march=native -O2 -Wall -std=gnu11
CXX = g++
CXXFLAGS = -march=native -O2 -Wall -std=gnu++11
CPPFLAGS = -I.
LDFLAGS = -lm -lncurses

# Make variables
VPATH =

# Shell Commands
RM_FILE = rm -vf
RM_DIR = rm -rvf
CP_FILE = cp -v
CP_DIR = cp -Rv
INST_FILE = install -Dm644 -o root -g root -v
INST_BIN = install -Dm755 -o root -g root -v
INST_DIR = install -dm755 -o root -g root -v
INST_USRFILE = install -Dm644 -o $$USER -g $$USER -v
INST_USRBIN = install -Dm755 -o $$USER -g $$USER -v
INST_USRDIR = install -dm755 -o $$USER -g $$USER -v
MKDIR = mkdir -v

# Program Sources and Dependencies
SOURCES = player_move_main.c ../draw.c ../moves.c
OBJ = $(SOURCES:.cxx=.o)
HDRS = ../draw.h ../moves.h
BINFILE = player_move_main

$(BINFILE): $(OBJ) $(HDRS)
	@echo
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $(OBJ) $(LDFLAGS)

%.o: %.cxx $(HDRS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	$(RM_FILE) $(OBJ) $(BINFILE)

