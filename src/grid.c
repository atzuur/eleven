#include "grid.h"
#include "raylib.h"

Grid GridCreate(Vector2i size, Vector2i tileSize, int spacing) {

    srand(time(0)); // for `GridAddRandomTile`

    Grid grid = {0};
    grid.size = size;
    grid.tileSize = tileSize;
    grid.spacing = spacing;

    grid.tiles = malloc(sizeof(Tile*) * grid.size.x);
    for (int x = 0; x < grid.size.x; x++) {
        grid.tiles[x] = malloc(sizeof(Tile) * grid.size.y);
    }

    int gridSize = GridSize(grid);
    grid.freeTiles = malloc(gridSize * sizeof(int));
    grid.nFreeTiles = gridSize;

    Vector2i gridOrigin = GridOrigin(grid);

    for (int x = 0; x < grid.size.x; x++) {
        for (int y = 0; y < grid.size.y; y++) {

            grid.tiles[x][y].value = 0;
            grid.tiles[x][y].visible = false;

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

void GridReset(Grid* grid) {

    for (int x = 0; x < grid->size.x; x++) {
        for (int y = 0; y < grid->size.y; y++) {
            grid->tiles[x][y].value = 0;
            grid->tiles[x][y].visible = false;
        }
    }
}

void GridAddRandomTile(Grid* grid) {

    int index = rand() % grid->nFreeTiles;
    Vector2i pos = GridIndexToPos(*grid, index);
    grid->tiles[pos.x][pos.y].visible = true;
}

bool GridIsFull(Grid grid) {

    for (int x = 0; x < grid.size.x; x++) {
        for (int y = 0; y < grid.size.y; y++) {
            if (!grid.tiles[x][y].visible)
                return false;
        }
    }

    return true;
}

void GridMove(Grid* grid, GridDirection direction) {

    for (int x = 0; x < grid->size.x; x++) {
        for (int y = 0; y < grid->size.y; y++) {
            // todo
        }
    }
}