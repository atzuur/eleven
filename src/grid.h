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
} Tile;

typedef struct {
    Vector2i size;
    Vector2i tileSize;
    int spacing;
    Tile** tiles;
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

// create a new grid
// `dims`: the number of tiles in each dimension
// `tileSize`: the size of each tile in pixels
// `spacing`: the spacing between tiles in pixels
Grid GridCreate(Vector2i dims, Vector2i tileSize, int spacing);

// free all memory allocated for the grid
void GridDestroy(Grid* grid);

// reset the grid, i.e. remove all tiles
void GridReset(Grid* grid);

// move all tiles in a direction, merging tiles with the same value
void GridMove(Grid* grid, GridDirection direction);

// make a tile appear in a random position, containing either 2 or 4
void GridAddRandomTile(Grid* grid);

// check if the grid is full, i.e. no more tiles can be added
bool GridIsFull(Grid grid);
