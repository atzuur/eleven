#include <raylib.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} Vector2i;

#define SQUARE_MAX UINTMAX_MAX

typedef struct {
    uintmax_t value;
    bool visible;
    Vector2i screenPos;
} Square;

typedef struct {
    Vector2i size;
    Vector2i squareSize;
    int spacing;
    Square** squares;
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
        grid.size.x * grid.squareSize.x + (grid.size.x - 1) * grid.spacing,
        grid.size.y * grid.squareSize.y + (grid.size.y - 1) * grid.spacing,
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
        pos.x * (grid.squareSize.x + grid.spacing),
        pos.y * (grid.squareSize.y + grid.spacing),
    };
}

Grid GridCreate(Vector2i size, Vector2i squareSize, int spacing);
void GridDestroy(Grid* grid);