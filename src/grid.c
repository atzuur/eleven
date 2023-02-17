#include "grid.h"

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

    for (int i = 0; i < grid.nFreeTiles; i++) {
        grid.freeTiles[i] = i;
    }

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

    int index = grid->freeTiles[rand() % grid->nFreeTiles];
    Vector2i pos = GridIndexToPos(*grid, index);

    grid->tiles[pos.x][pos.y].visible = true;
    grid->tiles[pos.x][pos.y].value = (rand() % 10) < 9 ? 2 : 4; // 10% chance of 4
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

            Tile* curTile = &grid->tiles[x][y];
            if (!curTile->visible)
                continue;

            Vector2i curPos = (Vector2i) {x, y};

            while (1) {
                Vector2i nextPos = GridTileAdjacentTo(*grid, curPos, direction);
                if (nextPos.x < 0 || nextPos.y < 0) // already on the edge
                    break;

                Tile* nextTile = &grid->tiles[nextPos.x][nextPos.y];

                if (nextTile->visible && nextTile->value == curTile->value) {

                    GridStepTile(grid, curPos, direction);
                    curPos = nextPos;
                    nextTile->value *= 2;
                    continue;

                } else if (nextTile->visible) {
                    break;
                }

                GridStepTile(grid, curPos, direction);
                curPos = nextPos;
            }
        }
    }
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

    GridUpdateFreeTiles(grid);
}

void GridUpdateFreeTiles(Grid* grid) {

    grid->nFreeTiles = 0;

    for (int x = 0; x < grid->size.x; x++) {
        for (int y = 0; y < grid->size.y; y++) {

            if (!grid->tiles[x][y].visible) {
                int freeIndex = GridPosToIndex(*grid, (Vector2i) {x, y});
                grid->freeTiles[grid->nFreeTiles] = freeIndex;
                grid->nFreeTiles++;
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
