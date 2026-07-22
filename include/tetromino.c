#include <raylib.h>
#include <definitions.h>
#include <tetromino.h>


void draw_tetromino_code(tetromino_shapes code, RenderTexture2D* Render, Texture2D* block) {
       BeginTextureMode(*Render);
       switch (code) {
        case I:
            DrawTexture(*block, 0, 0, WHITE);
            DrawTexture(*block, block_width, 0, WHITE);
            DrawTexture(*block, 2 * block_width, 0, WHITE);
            DrawTexture(*block, 3 * block_width, 0, WHITE);
            break;
        case J:
            DrawTexture(*block, 0, 0, WHITE);
            DrawTexture(*block, 0, block_height, WHITE);
            DrawTexture(*block, block_width, block_height, WHITE);
            DrawTexture(*block, 2 * block_width, block_height, WHITE);
            break;
        case L:
            DrawTexture(*block, 2 * block_width, 0, WHITE);
            DrawTexture(*block, 0, block_height, WHITE);
            DrawTexture(*block, block_width, block_height, WHITE);
            DrawTexture(*block, 2 * block_width, block_height, WHITE);
            break;
        case O:
            DrawTexture(*block, 0, 0, WHITE);
            DrawTexture(*block, block_width, 0, WHITE);
            DrawTexture(*block, 0, block_height, WHITE);
            DrawTexture(*block, block_width, block_height, WHITE);
            break;
        case S:
            DrawTexture(*block, block_width, 0, WHITE);
            DrawTexture(*block, 2 * block_width, 0, WHITE);
            DrawTexture(*block, 0, block_height, WHITE);
            DrawTexture(*block, block_width, block_height, WHITE);
            break;
        case T:
            DrawTexture(*block, block_width, 0, WHITE);
            DrawTexture(*block, 0, block_height, WHITE);
            DrawTexture(*block, block_width, block_height, WHITE);
            DrawTexture(*block, 2 * block_width, block_height, WHITE);
            break;
        case Z:
            DrawTexture(*block, 0, 0, WHITE);
            DrawTexture(*block, block_width, 0, WHITE);
            DrawTexture(*block, block_width, block_height, WHITE);
            DrawTexture(*block, 2 * block_width, block_height, WHITE);
            break;
    };
    EndTextureMode();
}

int get_tetromino_height(tetromino_shapes shape) {
    switch (shape) {
        case I: return block_height;         // 1 block tall
        case O: return 2 * block_height;     // 2 blocks tall
        default: return 2 * block_height;    // J, L, S, T, Z
    }
}

int get_tetromino_width(tetromino_shapes shape) {
    switch (shape) {
        case I: return 4 * block_width;      // 4 blocks wide
        case O: return 2 * block_width;      // 2 blocks wide
        default: return 3 * block_width;     // J, L, S, T, Z
    }
}
