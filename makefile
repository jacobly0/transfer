# ----------------------------
# Makefile Options
# ----------------------------

CC = clang
CONVERT = convert
PRINTF = printf

NAME = TRANSFER
ICON = icon/transfer.png
DESCRIPTION = "Variable Transfer Program"
COMPRESSED = YES
ARCHIVED = YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

ICON_COLORS = 16
ICON_SIZES = 32 64

GEN_CSOURCES = src/ti83pce_icon.c src/ti84pce_icon.c src/font.c
EXTRA_CSOURCES = $(filter-out $(wildcard $(GEN_CSOURCES)),$(GEN_CSOURCES))
EXTRA_USERHEADERS = src/ti83pce_icon.h src/ti84pce_icon.h src/ti84pceg.inc src/font.h
EXTRA_CLEAN = $(GEN_CSOURCES) $(patsubst %.c,%.h,$(GEN_CSOURCES)) icon/ti83pce.ico icon/ti84pce.ico font/genfont

# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk

icon/%.ico: $(patsubst %,icon/\%-%.ico,$(ICON_SIZES))
	@$(CONVERT) $^ -colors $(ICON_COLORS) $@

src/%_icon.h: icon/%.ico
	@$(PRINTF) "#ifndef $*_icon_h\n#define $*_icon_h\n\n#define $*_icon_uncompressed_size `wc -c < $<`\nextern const unsigned char $*_icon[];\n\n#endif\n" > $@

src/%_icon.c: icon/%.ico
	@$(CONVBIN) --iformat bin --input $< --compress zx7 --name $*_icon --oformat c --output $@

src/font.h src/font.c: font/genfont
	@$<

font/genfont: font/genfont.c
	@$(CC) -O3 -flto $< `pkg-config --cflags --libs freetype2` -o $@

.SECONDARY: $(EXTRA_CLEAN)
