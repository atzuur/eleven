#pragma once

#include "grid.h"
#include <inttypes.h>
#include <raylib.h>

void EvDrawTile(const Tile* tile, Vector2i tileSize, Color tileColor);

void EvDrawEndScreen(void);

void EvDrawScoreText(uintmax_t score);

void EvDrawGrid(const Grid* grid);

Color EvGetTileColor(uintmax_t value);
