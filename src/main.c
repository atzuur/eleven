#include "grid.h"
#include <inttypes.h>
#include <stdio.h>

void EvDrawTile(Tile* tile, Vector2i tileSize) {

    DrawRectangle(tile->screenPos.x, tile->screenPos.y, tileSize.x, tileSize.y, BEIGE);

    snprintf(tile->text, TILE_MAX_DIGITS, "%" PRIuMAX, tile->value);
    Vector2i textPos = {tile->screenPos.x / 2, tile->screenPos.y / 2};
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

                GridMove(&grid, EvGetDirection(key));
                GridAddRandomTile(&grid);
            } break;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int x = 0; x < grid.size.x; x++) {
            for (int y = 0; y < grid.size.y; y++) {

                Tile* tile = &grid.tiles[x][y];

                if (tile->visible) {
                    EvDrawTile(tile, grid.tileSize);
                }
            }
        }

        EndDrawing();
    }

    GridDestroy(&grid);
    CloseWindow();

    return 0;
}
