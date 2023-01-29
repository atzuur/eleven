#include "grid.h"

Grid GridCreate(Vector2i size, Vector2i squareSize, int spacing) {

    Grid grid = {0};
    grid.size = size;
    grid.squareSize = squareSize;
    grid.spacing = spacing;

    grid.squares = malloc(sizeof(Square*) * grid.size.x);
    for (int x = 0; x < grid.size.x; x++) {
        grid.squares[x] = malloc(sizeof(Square) * grid.size.y);
    }

    Vector2i gridOrigin = GridOrigin(grid);

    for (int x = 0; x < grid.size.x; x++) {
        for (int y = 0; y < grid.size.y; y++) {

            grid.squares[x][y].value = 0;
            grid.squares[x][y].visible = true;

            Vector2i stride = GridStridePixels(grid, (Vector2i) {x, y});
            grid.squares[x][y].screenPos.x = gridOrigin.x + stride.x;
            grid.squares[x][y].screenPos.y = gridOrigin.y + stride.y;
        }
    }

    return grid;
}

void GridDestroy(Grid* grid) {

    for (int x = 0; x < grid->size.x; x++) {
        free(grid->squares[x]);
        grid->squares[x] = NULL;
    }

    free(grid->squares);
    grid->squares = NULL;
}
