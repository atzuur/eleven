#include "grid.h"
#include "raylib.h"
#include <inttypes.h>
#include <stdio.h>

const char* const GridDirectionStrs[] = {
    "up",
    "down",
    "left",
    "right",
};

void EvDrawTile(Tile* tile, Vector2i tileSize) {

    DrawRectangle(tile->screenPos.x, tile->screenPos.y, tileSize.x, tileSize.y, BEIGE);

    snprintf(tile->text, TILE_MAX_DIGITS, "%" PRIuMAX, tile->value);
    Vector2i textPos = {tile->screenPos.x + tileSize.x / 2,
                        tile->screenPos.y + tileSize.y / 2};

    DrawText(tile->text, textPos.x, textPos.y, 20, BLACK);
}

GridDirection EvGetDirection(KeyboardKey key) {

    switch (key) {

        case KEY_UP: {
            return GRID_UP;
        } break;

        case KEY_DOWN: {
            return GRID_DOWN;
        } break;

        case KEY_LEFT: {
            return GRID_LEFT;
        } break;

        case KEY_RIGHT: {
            return GRID_RIGHT;
        } break;

        default: {
            return -1;
        } break;
    }
}

int main(void) {

    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "eleven");
    SetTargetFPS(60);

    Vector2i gridSize = {4, 4};
    Vector2i tileSize = {100, 100};
    int spacing = 10;

    Grid grid = GridCreate(gridSize, tileSize, spacing);

    // add 2 initial tiles
    GridAddRandomTile(&grid);
    GridAddRandomTile(&grid);

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_ESCAPE)) {
            break;
        }

        if (GridIsFull(grid)) {
            DrawText("Game Over!", GetScreenWidth() / 2, GetScreenWidth() / 2, 72,
                     DARKGRAY);
            break;
        }

        int key = GetKeyPressed();
        switch (key) {

            case KEY_R: {
                GridReset(&grid);
            } break;

            case KEY_RIGHT:
            case KEY_LEFT:
            case KEY_UP:
            case KEY_DOWN: {
                GridDirection dir = EvGetDirection(key);
                GridMove(&grid, dir);
                printf("moved %s\n", GridDirectionStrs[dir]);
                GridAddRandomTile(&grid);
            } break;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        Vector2i origin = GridOrigin(grid);
        Vector2i size = GridSizePixels(grid);
        Vector2i halfTile = {grid.tileSize.x / 2, grid.tileSize.y / 2};

        // fill the background
        DrawRectangle(origin.x, origin.y, size.x, size.y, BROWN);

        for (int x = 0; x < grid.size.x; x++) {

            Vector2i xPos = GridPosToPixels(grid, (Vector2i) {x, 0});
            DrawText(TextFormat("%d", x), xPos.x + halfTile.x, xPos.y - halfTile.y, 20,
                     BLACK);

            for (int y = 0; y < grid.size.y; y++) {

                Tile* tile = &grid.tiles[x][y];

                if (tile->visible) {
                    EvDrawTile(tile, grid.tileSize);
                }

                if (x == 0) {
                    Vector2i yPos = GridPosToPixels(grid, (Vector2i) {0, y});
                    DrawText(TextFormat("%d", y), yPos.x - halfTile.x,
                             yPos.y + halfTile.y, 20, BLACK);
                }
            }
        }

        EndDrawing();
    }

    GridDestroy(&grid);
    CloseWindow();

    return 0;
}
