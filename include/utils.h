#ifndef UTILS
#define UTILS

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

typedef struct tetromino {
    int width;
    int height;
    tetromino_shapes code;
    const char* texture_path;
} tetromino;

#endif