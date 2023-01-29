#include "grid.h"

Grid GridCreate(Vector2i size, Vector2i tileSize, int spacing) {

    Grid grid = {0};
    grid.size = size;
    grid.tileSize = tileSize;
    grid.spacing = spacing;

    grid.tiles = malloc(sizeof(Tile*) * grid.size.x);
    for (int x = 0; x < grid.size.x; x++) {
        grid.tiles[x] = malloc(sizeof(Tile) * grid.size.y);
    }

    Vector2i gridOrigin = GridOrigin(grid);

    for (int x = 0; x < grid.size.x; x++) {
        for (int y = 0; y < grid.size.y; y++) {

            grid.tiles[x][y].value = 0;
            grid.tiles[x][y].visible = true;

            Vector2i stride = GridStridePixels(grid, (Vector2i) {x, y});
            grid.tiles[x][y].screenPos.x = gridOrigin.x + stride.x;
            grid.tiles[x][y].screenPos.y = gridOrigin.y + stride.y;
        }
    }

    return grid;
}

void GridDestroy(Grid* grid) {

    for (int x = 0; x < grid->size.x; x++) {
        free(grid->tiles[x]);
        grid->tiles[x] = NULL;
    }

    free(grid->tiles);
    grid->tiles = NULL;
}
