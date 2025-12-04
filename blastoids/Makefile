CC ?= cc
PKG_PACKAGES := allegro-5 allegro_main-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5 allegro_image-5

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

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

run: all
	./$(TARGET)

clean:
	@rm -rf $(OBJDIR) $(TARGET)

check-pkg:
	@if [ -z "$(PKG_CFLAGS)" ] || [ -z "$(PKG_LIBS)" ]; then \
		echo "ERROR: pkg-config could not find required Allegro packages."; \
		echo "Install allegro and pkg-config (Homebrew: brew install pkg-config allegro)."; \
		false; \
	else \
		echo "pkg-config found Allegro packages:"; \
		pkg-config --modversion allegro-5 || true; \
	fi
