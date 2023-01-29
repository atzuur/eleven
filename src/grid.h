#include <raylib.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} Vector2i;

#define tile_MAX UINTMAX_MAX

typedef struct {
    uintmax_t value;
    bool visible;
    Vector2i screenPos;
} tile;

typedef struct {
    Vector2i size;
    Vector2i tileSize;
    int spacing;
    tile** tiles;
} Grid;

typedef enum {
    GRID_UP,
    GRID_DOWN,
    GRID_LEFT,
    GRID_RIGHT,
} GridDirection;

inline int GridSize(Grid grid) {
    return grid.size.x * grid.size.y;
}

inline Vector2i GridSizePixels(Grid grid) {
    return (Vector2i) {
        grid.size.x * grid.tileSize.x + (grid.size.x - 1) * grid.spacing,
        grid.size.y * grid.tileSize.y + (grid.size.y - 1) * grid.spacing,
    };
}

inline Vector2i GridOrigin(Grid grid) {
    Vector2i screenCenter = {GetScreenWidth() / 2, GetScreenHeight() / 2};
    Vector2i gridPixels = GridSizePixels(grid);
    return (Vector2i) {
        screenCenter.x - gridPixels.x / 2,
        screenCenter.y - gridPixels.y / 2,
    };
}

inline Vector2i GridStridePixels(Grid grid, Vector2i pos) {
    return (Vector2i) {
        pos.x * (grid.tileSize.x + grid.spacing),
        pos.y * (grid.tileSize.y + grid.spacing),
    };
}

Grid GridCreate(Vector2i size, Vector2i tileSize, int spacing);
void GridDestroy(Grid* grid);
void GridMove(Grid* grid, GridDirection direction);