#include <raylib.h>
#include <definitions.h>
#include <tetromino.h>

// Base cell positions for each shape at rotation=0 (4 blocks each)
// I-piece uses a 4x4 bounding box, O-piece uses 2x2, others use 3x3
static const int CELL_COUNT[7] = {4, 4, 4, 4, 4, 4, 4};
static const int GRID_SIZE[7]  = {4, 3, 3, 2, 3, 3, 3};

// (x, y) pairs for each shape at rotation 0
typedef struct { int x; int y; } Cell;
static const Cell SHAPE_CELLS[7][4] = {
    // I: ####
    [I] = {{0,1}, {1,1}, {2,1}, {3,1}},
    // J: # / ###
    [J] = {{0,0}, {0,1}, {1,1}, {2,1}},
    // L:   # / ###
    [L] = {{2,0}, {0,1}, {1,1}, {2,1}},
    // O: ## / ##
    [O] = {{0,0}, {1,0}, {0,1}, {1,1}},
    // S:  ## / ##
    [S] = {{1,0}, {2,0}, {0,1}, {1,1}},
    // T:  # / ###
    [T] = {{1,0}, {0,1}, {1,1}, {2,1}},
    // Z: ## /  ##
    [Z] = {{0,0}, {1,0}, {1,1}, {2,1}},
};

/// Rotate (x,y) CW by 90° within a grid of size `n`
static inline Cell rotate_cw(Cell c, int n) {
    return (Cell){ n - 1 - c.y, c.x };
}

/// Rotate (x,y) CCW by 90° within a grid of size `n`
static inline Cell rotate_ccw(Cell c, int n) {
    return (Cell){ c.y, n - 1 - c.x };
}

/// Apply rotation 0,1,2,3 times (CW) to a cell
static Cell transform_cell(Cell c, int rotation, int n) {
    for (int i = 0; i < rotation; i++) {
        c = rotate_cw(c, n);
    }
    return c;
}

void draw_tetromino_code(tetromino_shapes code, int rotation, RenderTexture2D* Render, Texture2D* block) {
    int n = GRID_SIZE[code];
    BeginTextureMode(*Render);
    ClearBackground(BLANK);
    // compute rotated cells and find min offset for normalization
    Cell cells[4];
    int min_x = 999, min_y = 999;
    for (int i = 0; i < CELL_COUNT[code]; i++) {
        cells[i] = transform_cell(SHAPE_CELLS[code][i], rotation, n);
        if (cells[i].x < min_x) min_x = cells[i].x;
        if (cells[i].y < min_y) min_y = cells[i].y;
    }
    // normalize to zero-origin and draw
    for (int i = 0; i < CELL_COUNT[code]; i++) {
        DrawTexture(*block,
                    (cells[i].x - min_x) * block_width,
                    (cells[i].y - min_y) * block_height,
                    WHITE);
    }
    EndTextureMode();
}

int get_tetromino_height(tetromino_shapes shape, int rotation) {
    int n = GRID_SIZE[shape];
    int min_y = 999, max_y = -1;
    for (int i = 0; i < CELL_COUNT[shape]; i++) {
        Cell tc = transform_cell(SHAPE_CELLS[shape][i], rotation, n);
        if (tc.y < min_y) min_y = tc.y;
        if (tc.y > max_y) max_y = tc.y;
    }
    return (max_y - min_y + 1) * block_height;
}

int get_tetromino_width(tetromino_shapes shape, int rotation) {
    int n = GRID_SIZE[shape];
    int min_x = 999, max_x = -1;
    for (int i = 0; i < CELL_COUNT[shape]; i++) {
        Cell tc = transform_cell(SHAPE_CELLS[shape][i], rotation, n);
        if (tc.x < min_x) min_x = tc.x;
        if (tc.x > max_x) max_x = tc.x;
    }
    return (max_x - min_x + 1) * block_width;
}

void get_tetromino_cells(tetromino_shapes shape, int rotation, int out_cells[4][2]) {
    int n = GRID_SIZE[shape];
    Cell cells[4];
    int min_x = 999, min_y = 999;
    for (int i = 0; i < CELL_COUNT[shape]; i++) {
        cells[i] = transform_cell(SHAPE_CELLS[shape][i], rotation, n);
        if (cells[i].x < min_x) min_x = cells[i].x;
        if (cells[i].y < min_y) min_y = cells[i].y;
    }
    for (int i = 0; i < CELL_COUNT[shape]; i++) {
        out_cells[i][0] = cells[i].x - min_x;
        out_cells[i][1] = cells[i].y - min_y;
    }
}
