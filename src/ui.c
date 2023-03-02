#include "ui.h"
#include <stdio.h>

const int tileFontSize = 20;
const int uiFontSize = 30;

void EvDrawTile(const Tile* tile, Vector2i tileSize, Color tileColor) {

    DrawRectangle(tile->screenPos.x, tile->screenPos.y, tileSize.x, tileSize.y,
                  tileColor);

    snprintf((char*)tile->text, TILE_MAX_DIGITS, "%" PRIuMAX, tile->value);

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

void EvDrawScoreText(uintmax_t score) {

    int screenWidth = GetScreenWidth();

    const char* scoreText = TextFormat("Score: %" PRIuMAX, score);
    int scoreTextWidth = MeasureText(scoreText, uiFontSize);
    Vector2i corner = {screenWidth - 10 - scoreTextWidth, 10};

    DrawText(TextFormat("Score: %" PRIuMAX, score), corner.x, corner.y, uiFontSize,
             DARKGRAY);
}

void EvDrawGrid(const Grid* grid) {

    Vector2i origin = GridOrigin(*grid);
    Vector2i size = GridSizePixels(*grid);

    // fill in gaps between tiles
    DrawRectangle(origin.x, origin.y, size.x, size.y, LIGHTGRAY);

    for (int x = 0; x < grid->size.x; x++) {
        for (int y = 0; y < grid->size.y; y++) {

            const Tile* tile = &grid->tiles[x][y];

            if (tile->visible) {
                EvDrawTile(tile, grid->tileSize, EvGetTileColor(tile->value));
            }
        }
    }
}

Color EvGetTileColor(uintmax_t value) {

    switch (value) {
        case 0:
            return (Color) {197, 185, 172, 255};
        case 2:
            return (Color) {231, 221, 211, 255};
        case 4:
            return (Color) {230, 217, 193, 255};
        case 8:
            return (Color) {235, 170, 114, 255};
        case 16:
            return (Color) {238, 142, 92, 255};
        case 32:
            return (Color) {239, 117, 88, 255};
        case 64:
            return (Color) {239, 87, 52, 255};
        case 128:
            return (Color) {230, 200, 107, 255};
        case 256:
            return (Color) {230, 197, 90, 255};
        case 512:
            return (Color) {230, 193, 73, 255};
        case 1024:
            return (Color) {230, 190, 56, 255};
        case 2048:
            return (Color) {230, 187, 39, 255};
        default:
            return (Color) {53, 51, 43, 255};
    }
}
