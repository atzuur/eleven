CC := gcc
CFLAGS := -Wall -Wextra -pedantic -std=c99 -O3 -Iraylib/src
LDLIBS := -lopengl32 -lgdi32 -lwinmm -lkernel32

RAYLIB_BIN := raylib/src/libraylib.a
FILES := $(wildcard src/*.c) $(RAYLIB_BIN) $(LDLIBS)

all: $(RAYLIB_BIN)
	$(CC) $(CFLAGS) $(FILES) -o eleven

$(RAYLIB_BIN):
ifeq (,$(wildcard $(RAYLIB_BIN))) # if raylib is not built yet
	$(MAKE) -C raylib/src CUSTOM_CFLAGS="-Wno-unused-function" -j6
endif

clean:
	$(MAKE) -C raylib/src clean