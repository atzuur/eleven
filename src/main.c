#include "grid.h"
#include <inttypes.h>
#include <stdio.h>

const int tileFontSize = 20;
const int uiFontSize = 30;

const int screenWidth = 800;
const int screenHeight = 800;

const Vector2i tileSize = {100, 100};
const int tileSpacing = 10;

void EvDrawTile(Tile* tile, Vector2i tileSize, Color tileColor) {

    DrawRectangle(tile->screenPos.x, tile->screenPos.y, tileSize.x, tileSize.y,
                  tileColor);

    snprintf(tile->text, TILE_MAX_DIGITS, "%" PRIuMAX, tile->value);
    Vector2i textPos = {tile->screenPos.x + tileSize.x / 2,
                        tile->screenPos.y + tileSize.y / 2};

    DrawText(tile->text, textPos.x, textPos.y, tileFontSize, BLACK);
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

void EvResetGrid(Grid* grid) {

    GridReset(grid);
    GridAddRandomTile(grid);
    GridAddRandomTile(grid);
}

int main(void) {

    InitWindow(screenWidth, screenHeight, "eleven");
    SetTargetFPS(60);

    const Vector2i gridSize = {4, 4};

    Grid grid = GridCreate(gridSize, tileSize, tileSpacing);
    EvResetGrid(&grid);

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
                EvResetGrid(&grid);
            } break;

            case KEY_RIGHT:
            case KEY_LEFT:
            case KEY_UP:
            case KEY_DOWN: {
                GridMove(&grid, EvGetDirection(key));
                GridAddRandomTile(&grid);
            } break;
        }

        const Vector2i origin = GridOrigin(grid);
        const Vector2i size = GridSizePixels(grid);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        const char* scoreText = TextFormat("Score: %" PRIuMAX, grid.score);
        int scoreTextWidth = MeasureText(scoreText, 30);
        Vector2i corner = {screenWidth - 10 - scoreTextWidth, 10};

        DrawText(TextFormat("Score: %" PRIuMAX, grid.score), corner.x, corner.y, 30,
                 DARKGRAY);

        // fill the background
        DrawRectangle(origin.x, origin.y, size.x, size.y, LIGHTGRAY);

        for (int x = 0; x < grid.size.x; x++) {
            for (int y = 0; y < grid.size.y; y++) {

                Tile* tile = &grid.tiles[x][y];

                if (tile->visible) {
                    EvDrawTile(tile, grid.tileSize, BEIGE);
                }
            }
        }

        EndDrawing();
    }

    GridDestroy(&grid);
    CloseWindow();

    return 0;
}
