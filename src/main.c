#include "grid.h"
#include <raylib.h>

int main(void) {

    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "eleven");
    SetTargetFPS(60);

    Vector2i gridSize = {4, 4};
    Vector2i squareSize = {100, 100}; // pixels
    int spacing = 10; // pixels

    Grid grid = GridCreate(gridSize, squareSize, spacing);

    while (!WindowShouldClose()) {

        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int x = 0; x < grid.size.x; x++) {
            for (int y = 0; y < grid.size.y; y++) {

                Square* square = &grid.squares[x][y];

                if (square->visible) {
                    DrawRectangle(square->screenPos.x, square->screenPos.y,
                                  grid.squareSize.x, grid.squareSize.y, RED);
                }
            }
        }

        EndDrawing();
    }

    GridDestroy(&grid);
    CloseWindow();

    return 0;
}