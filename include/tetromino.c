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