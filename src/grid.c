#include "grid.h"
#include <stdlib.h>
#include <time.h>

Grid GridCreate(Vector2i size, Vector2i tileSize) {

    srand(time(0)); // for `GridAddRandomTile`

    Grid grid = {0};
    grid.size = size;
    grid.tileSize = tileSize;

    int max_spacing = MIN(tileSize.x, tileSize.y) / 10;
    grid.spacing = MAX(max_spacing, 2);

    grid.tiles = malloc(sizeof(Tile*) * grid.size.x);
    for (int x = 0; x < grid.size.x; x++) {
        grid.tiles[x] = calloc(grid.size.y, sizeof(Tile));
    }

    int gridSize = GridSize(grid);

    grid.freeTiles = malloc(gridSize * sizeof(int));
    grid.nFreeTiles = gridSize;

    for (int i = 0; i < grid.nFreeTiles; i++) {
        grid.freeTiles[i] = i;
    }

    Vector2i gridOrigin = GridOrigin(grid);

    for (int x = 0; x < grid.size.x; x++) {
        for (int y = 0; y < grid.size.y; y++) {

            Tile* tile = &grid.tiles[x][y];

            Vector2i stride = GridStridePixels(grid, (Vector2i) {x, y});
            tile->screenPos.x = gridOrigin.x + stride.x;
            tile->screenPos.y = gridOrigin.y + stride.y;
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
            Tile* tile = &grid->tiles[x][y];
            tile->visible = false;
            tile->value = 0;
            tile->alreadyMerged = false;
        }
    }

    grid->score = 0;
    GridUpdateFreeTiles(grid);
}

void GridAddRandomTile(Grid* grid) {

    int index = grid->freeTiles[rand() % grid->nFreeTiles];
    Vector2i pos = GridIndexToPos(*grid, index);

    grid->tiles[pos.x][pos.y].visible = true;
    grid->tiles[pos.x][pos.y].value = (rand() % 10) < 9 ? 2 : 4; // 10% chance of 4

    GridUpdateFreeTiles(grid);
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

    // reset merge flags
    for (int x = 0; x < grid->size.x; x++) {
        for (int y = 0; y < grid->size.y; y++) {
            grid->tiles[x][y].alreadyMerged = false;
        }
    }

    Vector2i from = {0};
    Vector2i to = grid->size;
    int iter = 1;

    // reverse direction
    if (direction == GRID_DOWN || direction == GRID_RIGHT) {
        from = (Vector2i) {grid->size.x - 1, grid->size.y - 1};
        to = (Vector2i) {-1, -1};
        iter = -1;
    }

    for (int x = from.x; x != to.x; x += iter) {
        for (int y = from.y; y != to.y; y += iter) {

            Tile* curTile = &grid->tiles[x][y];
            if (!curTile->visible)
                continue;

            Vector2i curPos = {x, y};

            while (1) {
                Vector2i nextPos = GridTileAdjacentTo(*grid, curPos, direction);
                if (nextPos.x < 0 || nextPos.y < 0) // already on the edge
                    break;

                Tile* nextTile = &grid->tiles[nextPos.x][nextPos.y];

                if (nextTile->visible && nextTile->value == curTile->value &&
                    !nextTile->alreadyMerged && !curTile->alreadyMerged) {

                    GridStepTile(grid, curPos, direction);
                    curPos = nextPos;
                    curTile = nextTile;

                    nextTile->value *= 2;
                    grid->score += nextTile->value;
                    nextTile->alreadyMerged = true;

                    continue;

                } else if (nextTile->visible) { // blocked
                    break;
                }

                GridStepTile(grid, curPos, direction);
                curPos = nextPos;
                curTile = nextTile;
            }
        }
    }

    grid->lastMoveTime = GetTime();
}

void GridStepTile(Grid* grid, Vector2i pos, GridDirection direction) {

    Vector2i destPos = GridTileAdjacentTo(*grid, pos, direction);
    if (destPos.x < 0 || destPos.y < 0)
        return;

    Tile* destTile = &grid->tiles[destPos.x][destPos.y];
    Tile* srcTile = &grid->tiles[pos.x][pos.y];
    *destTile = *srcTile;

    destTile->screenPos = GridPosToPixels(*grid, destPos);

    srcTile->visible = false;
    srcTile->value = 0;
    srcTile->alreadyMerged = false;

    GridUpdateFreeTiles(grid);
}

void GridUpdateFreeTiles(Grid* grid) {

    grid->nFreeTiles = 0;

    for (int x = 0; x < grid->size.x; x++) {
        for (int y = 0; y < grid->size.y; y++) {

            if (!grid->tiles[x][y].visible) {
                int freeIndex = GridPosToIndex(*grid, (Vector2i) {x, y});
                grid->freeTiles[grid->nFreeTiles++] = freeIndex;
            }
        }
    }
}

int GridSize(Grid grid) {
    return grid.size.x * grid.size.y;
}

Vector2i GridSizePixels(Grid grid) {
    return (Vector2i) {
        grid.size.x * grid.tileSize.x + (grid.size.x - 1) * grid.spacing,
        grid.size.y * grid.tileSize.y + (grid.size.y - 1) * grid.spacing,
    };
}

Vector2i GridOrigin(Grid grid) {
    Vector2i screenCenter = {GetScreenWidth() / 2, GetScreenHeight() / 2};
    Vector2i gridPixels = GridSizePixels(grid);
    return (Vector2i) {
        screenCenter.x - gridPixels.x / 2,
        screenCenter.y - gridPixels.y / 2,
    };
}

Vector2i GridStridePixels(Grid grid, Vector2i pos) {
    return (Vector2i) {
        pos.x * (grid.tileSize.x + grid.spacing),
        pos.y * (grid.tileSize.y + grid.spacing),
    };
}

Vector2i GridPosToPixels(Grid grid, Vector2i pos) {
    Vector2i origin = GridOrigin(grid);
    Vector2i stride = GridStridePixels(grid, pos);
    return (Vector2i) {
        origin.x + stride.x,
        origin.y + stride.y,
    };
}

Vector2i GridIndexToPos(Grid grid, int index) {
    return (Vector2i) {
        index % grid.size.x,
        index / grid.size.x,
    };
}

int GridPosToIndex(Grid grid, Vector2i pos) {
    return pos.y * grid.size.x + pos.x;
}

Vector2i GridTileAdjacentTo(Grid grid, Vector2i pos, GridDirection direction) {

    Vector2i adjacent = {0};

    switch (direction) {

        case GRID_UP: {
            adjacent = (Vector2i) {pos.x, pos.y - 1};
        } break;

        case GRID_DOWN: {
            adjacent = (Vector2i) {pos.x, pos.y + 1};
        } break;

        case GRID_LEFT: {
            adjacent = (Vector2i) {pos.x - 1, pos.y};
        } break;

        case GRID_RIGHT: {
            adjacent = (Vector2i) {pos.x + 1, pos.y};
        } break;
    }

    // check bounds
    if (adjacent.x < 0 || adjacent.x >= grid.size.x)
        return (Vector2i) {-1, -1};

    if (adjacent.y < 0 || adjacent.y >= grid.size.y)
        return (Vector2i) {-1, -1};

    return adjacent;
}
