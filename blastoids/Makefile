# Makefile for Blasteroids skeleton
# - builds all .c files in src/
# - puts .o files into obj/
# - links with Allegro via pkg-config
#
# Usage:
#   make          # build executable 'blasteroids'
#   make run      # build and run
#   make clean    # remove build artifacts
#   make check-pkg# verify pkg-config can find Allegro packages

CC ?= gcc
PKG_PACKAGES := allegro-5 allegro_main-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5 allegro_image-5 allegro_audio-5 allegro_acodec-5
PKG_CFLAGS := $(shell pkg-config --cflags $(PKG_PACKAGES) 2>/dev/null)
PKG_LIBS   := $(shell pkg-config --libs   $(PKG_PACKAGES) 2>/dev/null)

INCLUDE_DIR := include
SRCDIR := src
OBJDIR := obj
TARGET := blasteroids

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

CFLAGS := -Wall -Wextra -O2 $(PKG_CFLAGS) -I$(INCLUDE_DIR)
LDFLAGS := $(PKG_LIBS)

.PHONY: all clean run check-pkg

all: check-pkg $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compile to obj/
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ensure obj dir exists
$(OBJDIR):
	@mkdir -p $(OBJDIR)

# quick run target
run: all
	./$(TARGET)

clean:
	@rm -rf $(OBJDIR) $(TARGET)

# verify pkg-config found allegro packages
check-pkg:
	@if [ -z "$(PKG_CFLAGS)" ] || [ -z "$(PKG_LIBS)" ]; then \
		echo "ERROR: pkg-config could not find required Allegro packages."; \
		echo "Make sure allegro and pkg-config are installed and PKG_CONFIG_PATH is set."; \
		echo "You can try: brew install pkg-config allegro freetype libsndfile libogg libvorbis"; \
		false; \
	else \
		echo "pkg-config found Allegro packages:"; \
		pkg-config --modversion allegro-5 || true; \
		echo "CFLAGS: $(PKG_CFLAGS)"; \
		echo "LIBS: $(PKG_LIBS)"; \
	fi
