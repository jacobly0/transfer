# ----------------------------
# Makefile Options
# ----------------------------

CC ?= clang

NAME ?= TRANSFER
MAJOR_VERSION ?= 0
MINOR_VERSION ?= 0
PATCH_VERSION ?= 3
KIND_VERSION ?= n
BUILD_VERSION ?= -$(shell git rev-parse --short HEAD)
FULL_VERSION ?= v$(MAJOR_VERSION).$(MINOR_VERSION).$(PATCH_VERSION)$(KIND_VERSION)$(BUILD_VERSION)
ICON ?= transfer.png
DESCRIPTION ?= "Variable Transfer Program $(FULL_VERSION)"
COMPRESSED ?= YES
ARCHIVED ?= YES

FLAGS ?= -Wall -Wextra -Oz -DVERSION='"$(FULL_VERSION)"'
CFLAGS ?= $(FLAGS)
CXXFLAGS ?= $(FLAGS)

EXTRA_CSOURCES ?= src/font.c
EXTRA_USERHEADERS ?= src/ti84pceg.inc src/font.h
EXTRA_CLEAN ?= src/font.c src/font.h font/genfont

# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk

all:
	$(Q)echo [done] prgm$(NAME) $(FULL_VERSION)

beta: KIND_VERSION = b
beta: BUILD_VERSION =
beta: all

release: BUILD_VERSION =
release: REV_VERSION =
release: all

src/font.h src/font.c: font/genfont makefile
	$(Q)echo [running] $<
	$(Q)$<

font/genfont: font/genfont.c makefile
	$(Q)echo [compiling] $<
	$(Q)$(CC) -O3 -flto $< `pkg-config --cflags --libs freetype2` -o $@
