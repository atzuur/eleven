CC := gcc
CFLAGS_COMMON := -Wall -Wextra -pedantic -std=c99 -Iraylib/src
CFLAGS_DEBUG := -g3 -Og -DDEBUG
CFLAGS_RELEASE := -O3 -DNDEBUG
LDLIBS := -lopengl32 -lgdi32 -lwinmm -lkernel32

RAYLIB_BIN := raylib/src/libraylib.a
FILES := $(wildcard src/*.c) $(RAYLIB_BIN) $(LDLIBS)

.PHONY: release clean

release: $(RAYLIB_BIN)
	$(CC) $(CFLAGS_COMMON) $(CFLAGS_RELEASE) $(FILES) -o eleven

debug: $(RAYLIB_BIN)
	$(CC) $(CFLAGS_COMMON) $(CFLAGS_DEBUG) $(FILES) -o eleven

$(RAYLIB_BIN):
ifeq (,$(wildcard $(RAYLIB_BIN))) # if raylib is not built yet
	$(MAKE) -C raylib/src CUSTOM_CFLAGS="-w" -j4
endif

clean:
	$(MAKE) -C raylib/src clean
