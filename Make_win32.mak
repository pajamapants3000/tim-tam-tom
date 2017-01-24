#+\
# File   : tim-tam-tom.mk
# Program: tim-tam-tom -- A tic-tac-toe game written in curses
# Purpose: Makefile for tim-tam-tom
# Author : Tommy Lincoln <pajamapants3000@gmail.com>
# Notes  : Use this makefile for win32 systems; deps: NCurses
# License: MIT -- See LICENSE
#+/

# Compiler Flags
CC = gcc
CFLAGS += -march=native -O2 -Wall -std=gnu11
CPPFLAGS += -I. -I./include
LDFLAGS += -lm -lform -lncurses

# Make variables
VPATH  =
BINDIR = bin
LIBDIR = lib
INCDIR = include
OBJDIR = .obj

# Shell Commands
RM_FILE   = del
RM_DIR    = del /S
CP_FILE   = copy
CP_DIR    = copy
INST_FILE = copy
INST_BIN  = copy
INST_DIR  = copy
MKDIR     = mkdir

# Program Sources and Dependencies
SRC = tim-tam-tom.c moves.c draw.c form_username.c
_OBJ = $(SRC:.c=.o)
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))
_HDRS = moves.h draw.h form_username.h
HDRS = $(patsubst %,$(INCDIR)/%,$(_HDRS))
_BIN = tim-tam-tom.exe
BIN = $(patsubst %,$(BINDIR)/%,$(_BIN))

$(BIN): $(OBJ) $(HDRS)
	@echo
	if not exist $(BINDIR) $(MKDIR) $(BINDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $(OBJ) $(LDFLAGS)

$(OBJDIR)/%.o: %.c $(HDRS)
	@echo
	if not exist $(OBJDIR) $(MKDIR) $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	$(RM_FILE) $(OBJ) $(BIN)

