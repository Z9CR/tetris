#ifndef TETROMINO
#define TETROMINO
#include <raylib.h>

typedef enum tetromino_shapes {
    I = 0, // ####  I
    J = 1, // #
           // ###   J
    L = 2, //   #
           // ###   l
    O = 3, // ##
           // ##    O
    S = 4, //  ##
           // ##    S
    T = 5, //  #
           // ###   T
    Z = 6  // ##
           //  ##   Z
} tetromino_shapes;


typedef struct active_tetromino {
    tetromino_shapes shape;
    int rotation;       // 0=0°, 1=90°CW, 2=180°, 3=270°CW
    float pos_x;
    float pos_y;
    float fall_timer;
} active_tetromino;

typedef struct locked_tetromino {
    RenderTexture2D rt;
    tetromino_shapes shape;
    int rotation;
    float pos_x;
    float pos_y;
} locked_tetromino;

typedef struct tetromino {
    int width;
    int height;
    tetromino_shapes code;
    const char* texture_path;
} tetromino;

/// @param code a tetromino_shapes value
/// @param rotation 0-3 (CW 90° increments)
/// @param Render render target to draw onto
/// @param block texture of a single block
void draw_tetromino_code(tetromino_shapes code, int rotation, RenderTexture2D* Render, Texture2D* block);

/// @brief get tetromino pixel-height based on shape and rotation
int get_tetromino_height(tetromino_shapes shape, int rotation);

/// @brief get tetromino pixel-width based on shape and rotation
int get_tetromino_width(tetromino_shapes shape, int rotation);

/// @brief get the 4 normalized grid cell positions for a shape+rotation
/// @param out_cells[4][2] filled with (x,y) in block units, zero-origin
void get_tetromino_cells(tetromino_shapes shape, int rotation, int out_cells[4][2]);

#endif
