#include "grid.h"
#include <raylib.h>

int main(void) {

    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "eleven");
    SetTargetFPS(60);

    Vector2i gridSize = {4, 4};
    Vector2i tileSize = {100, 100}; // pixels
    int spacing = 10; // pixels

    Grid grid = GridCreate(gridSize, tileSize, spacing);

    while (!WindowShouldClose()) {

        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int x = 0; x < grid.size.x; x++) {
            for (int y = 0; y < grid.size.y; y++) {

                tile* tile = &grid.tiles[x][y];

                if (tile->visible) {
                    DrawRectangle(tile->screenPos.x, tile->screenPos.y,
                                  grid.tileSize.x, grid.tileSize.y, RED);
                }
            }
        }

        EndDrawing();
    }

    GridDestroy(&grid);
    CloseWindow();

    return 0;
}