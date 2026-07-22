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
    float pos_x;
    float pos_y;
    float fall_timer;
} active_tetromino;

typedef struct tetromino {
    int width;
    int height;
    tetromino_shapes code;
    const char* texture_path;
} tetromino;

/// @param code: a instant of `tetromino_shapes` 
/// @param Render: Render to contain tetrominos
/// @param block: randomized texture of tetrominos' block
void draw_tetromino_code(tetromino_shapes code, RenderTexture2D* Render, Texture2D* block);

/// @brief get tetromino pixel-height based on shape
int get_tetromino_height(tetromino_shapes shape);

/// @brief get tetromino pixel-width based on shape
int get_tetromino_width(tetromino_shapes shape);

#endif
