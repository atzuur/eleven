#include "grid.h"
#include "ui.h"

const Vector2i screenSize = {800, 600};

const Vector2i tileSize = {100, 100};
const int tileSpacing = 10;

static inline void EvResetGrid(Grid* grid) {

    GridReset(grid);
    GridAddRandomTile(grid);
    GridAddRandomTile(grid);
}

static inline GridDirection EvKeyToDirection(KeyboardKey key) {
    return key - KEY_RIGHT;
}

int main(void) {

    InitWindow(screenSize.x, screenSize.y, "eleven");
    SetTargetFPS(60);

    const Vector2i gridSize = {4, 4};

    Grid grid = GridCreate(gridSize, tileSize, tileSpacing);
    EvResetGrid(&grid);

    while (!WindowShouldClose()) {

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

    drawframe:
        BeginDrawing();

        ClearBackground(RAYWHITE);

        EvDrawScoreText(grid.score);
        EvDrawGrid(&grid);

        if (GridIsFull(grid)) {

            EvDrawEndScreen();
            if (GetTime() - grid.lastMoveTime >= 2.0) {
                EvResetGrid(&grid);
            } else {
                EndDrawing();
                goto drawframe;
            }
        }

        EndDrawing();
    }

    GridDestroy(&grid);
    CloseWindow();

    return 0;
}
