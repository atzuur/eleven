#include "grid.h"
#include "raylib.h"
#include <inttypes.h>
#include <stdio.h>

const int tileFontSize = 20;
const int uiFontSize = 30;

const Vector2i screenSize = {800, 600};

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

void EvDrawEndScreen(void) {

    char* gameOverText = "Game Over!";
    int fontSize = 84;

    int spacing = fontSize / 10; // font size / default font size (10)
    Vector2 textSize =
        MeasureTextEx(GetFontDefault(), gameOverText, fontSize, (float)spacing);

    Vector2i textPos = {GetScreenWidth() / 2 - (int)textSize.x / 2,
                        GetScreenHeight() / 2 - (int)textSize.y / 2};

    DrawText(gameOverText, textPos.x, textPos.y, fontSize, DARKGRAY);
}

void EvResetGrid(Grid* grid) {

    GridReset(grid);
    GridAddRandomTile(grid);
    GridAddRandomTile(grid);
}

GridDirection EvKeyToDirection(KeyboardKey key) {
    return key - KEY_RIGHT;
}

int main(void) {

    InitWindow(screenSize.x, screenSize.y, "eleven");
    SetTargetFPS(60);

    const Vector2i gridSize = {4, 4};

    Grid grid = GridCreate(gridSize, tileSize, tileSpacing);
    EvResetGrid(&grid);

    while (!WindowShouldClose()) {

        BeginDrawing();

        if (GridIsFull(grid)) {

            EvDrawEndScreen();
            EndDrawing();

            WaitTime(2.0);

            EvResetGrid(&grid);
            continue;
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
                GridMove(&grid, EvKeyToDirection(key));
                GridAddRandomTile(&grid);
            } break;
        }

        Vector2i origin = GridOrigin(grid);
        Vector2i size = GridSizePixels(grid);

        ClearBackground(RAYWHITE);

        const char* scoreText = TextFormat("Score: %" PRIuMAX, grid.score);
        int scoreTextWidth = MeasureText(scoreText, uiFontSize);
        Vector2i corner = {screenSize.x - 10 - scoreTextWidth, 10};

        DrawText(TextFormat("Score: %" PRIuMAX, grid.score), corner.x, corner.y,
                 uiFontSize, DARKGRAY);

        // fill in gaps between tiles
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
