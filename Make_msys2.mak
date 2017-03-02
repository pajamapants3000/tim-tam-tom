#+\
# File   : Make_msys2.mk
# Program: tim-tam-tom -- A tic-tac-toe game written in curses
# Purpose: Makefile for tim-tam-tom
# Author : Tommy Lincoln <pajamapants3000@gmail.com>
# License: MIT -- See LICENSE
#+/

# Compiler Flags
CC = gcc
CFLAGS := -march=native -O2 -Wall -std=gnu11 $(CFLAGS)
CXX = g++
CXXFLAGS := -march=native -O2 -Wall -std=gnu++11 $(CXXFLAGS)
CPPFLAGS := -I. -I./include -I/usr/include/ncurses $(CPPFLAGS)
LDFLAGS := -lm -lformw -lncurses $(LDFLAGS)

# Make variables
VPATH =
BINDIR = bin
LIBDIR = lib
INCDIR = include
OBJDIR = .obj

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
MKDIR = mkdir -pv

# Program Sources and Dependencies
SRC = tim-tam-tom.c moves.c draw.c form_username.c
_OBJ = $(SRC:.c=.o)
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))
_HDRS = moves.h draw.h form_username.h
HDRS = $(patsubst %,$(INCDIR)/%,$(_HDRS))
_BIN = tim-tam-tom
BIN = $(patsubst %,$(BINDIR)/%,$(_BIN))

$(BIN): $(OBJ) $(HDRS)
	@echo
	$(MKDIR) $(BINDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $(OBJ) $(LDFLAGS)

$(OBJDIR)/%.o: %.c $(HDRS)
	@echo
	$(MKDIR) $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	$(RM_FILE) $(OBJ) $(BIN)


