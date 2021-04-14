# ----------------------------
# Makefile Options
# ----------------------------

CC = clang

NAME ?= TRANSFER
ICON ?= transfer.png
DESCRIPTION ?= "Variable Transfer Program"
COMPRESSED ?= YES
ARCHIVED ?= YES

CFLAGS ?= -Wall -Wextra -Oz
CXXFLAGS ?= -Wall -Wextra -Oz

EXTRA_CSOURCES ?= $(if $(wildcard src/font.c),,src/font.c)
EXTRA_USERHEADERS ?= src/ti84pceg.inc src/font.h
EXTRA_CLEAN ?= src/font.c src/font.h font/genfont

# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk

src/font.h src/font.c: font/genfont
	@$<

font/genfont: font/genfont.c
	@$(CC) -O3 -flto $< `pkg-config --cflags --libs freetype2` -o $@
