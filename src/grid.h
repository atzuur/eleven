#include <raylib.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int x;
    int y;
} Vector2i;

#define STR(...) #__VA_ARGS__
#define XSTR(...) STR(__VA_ARGS__)

#define TILE_MAX UINTMAX_MAX
#define TILE_MAX_DIGITS (sizeof XSTR(UINTMAX_MAX))

typedef struct {
    uintmax_t value;
    bool visible;
    bool alreadyMerged; // avoid merging the same tile twice in a single move
    Vector2i screenPos;
    char text[TILE_MAX_DIGITS];
} Tile;

typedef struct {
    Vector2i size;
    Vector2i tileSize;
    int spacing;
    Tile** tiles;
    int* freeTiles; // array of indices of free tiles
    int nFreeTiles;
} Grid;

typedef enum {
    GRID_UP,
    GRID_DOWN,
    GRID_LEFT,
    GRID_RIGHT,
} GridDirection;

// get the number of tiles in the grid
int GridSize(Grid grid);

// get the size of the grid in pixels
Vector2i GridSizePixels(Grid grid);

// get the origin of the grid in pixels
Vector2i GridOrigin(Grid grid);

// get the screen position of the tile at the given grid position (relative to origin)
Vector2i GridStridePixels(Grid grid, Vector2i pos);

// convert a grid position to a screen position
Vector2i GridPosToPixels(Grid grid, Vector2i pos);

// convert a grid index to a grid position
Vector2i GridIndexToPos(Grid grid, int index);

// convert a grid position to a grid index
int GridPosToIndex(Grid grid, Vector2i pos);

// get the position of the tile adjacent to the tile at `pos` in the given `direction`
Vector2i GridTileAdjacentTo(Grid grid, Vector2i pos, GridDirection direction);

// create a new grid
// `dims`: the number of tiles in each dimension
// `tileSize`: the size of each tile in pixels
// `spacing`: the spacing between tiles in pixels
Grid GridCreate(Vector2i dims, Vector2i tileSize, int spacing);

// free all memory allocated for the grid
void GridDestroy(Grid* grid);

// reset the grid, i.e. remove all tiles
void GridReset(Grid* grid);

// move all tiles in a direction, merging tiles with the same value
void GridMove(Grid* grid, GridDirection direction);

// move tile `pos` one step towards `direction`
void GridStepTile(Grid* grid, Vector2i pos, GridDirection direction);

// make a tile appear in a random position, containing either 2 or 4
void GridAddRandomTile(Grid* grid);

// check if the grid is full, i.e. no more tiles can be added
bool GridIsFull(Grid grid);

// update Grid::freeTiles to contain the indices of all free tiles
void GridUpdateFreeTiles(Grid* grid);
