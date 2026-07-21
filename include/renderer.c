#include <definitions.h>
#include <utils.h>
#include <renderer.h>


/// @brief render the wall texture and return it
/// @param wall_img_path path to wall image
/// @return the rendered wall texture
RenderTexture2D wall_renderer(const char* wall_img_path) {
    Image _wall = LoadImage(wall_img_path);
    Texture2D wall = LoadTextureFromImage(_wall);
    RenderTexture2D wall_render = LoadRenderTexture(gamew, gameh);
    BeginTextureMode(wall_render);
    for (int i = 0; i <= gameh; i += gameh / wall_height) {
        DrawTexture(wall, 0, i, WHITE);
        DrawTexture(wall, gamew - block_width, i, WHITE);
    }
    EndTextureMode();
    UnloadTexture(wall);
    UnloadImage(_wall);
    return wall_render;
}


RenderTexture2D gen_next_block_texture(void)
{
    // gen random block shape
    const tetromino_shapes next_code = rand() % 7;
    tetromino next_prop;
    next_prop.texture_path = TextFormat("assets/blocks/%d.png", next_code);
    switch (next_code)
    {
    case I:
        next_prop.width = 4 * block_width;
        next_prop.height = block_height;
        break;
    case O:
        next_prop.width = 2 * block_width;
        next_prop.height = 2 * block_height;
        break;
    default:
        next_prop.width = 3 * block_width;
        next_prop.height = 2 * block_height;
        break;
    };
    // draw the tetromino
    RenderTexture2D next_shape = LoadRenderTexture(next_prop.width, next_prop.height);
    Image _block = LoadImage(next_prop.texture_path);
    Texture2D block = LoadTextureFromImage(_block);
    BeginTextureMode(next_shape);
    switch (next_code) {
        case I:
            DrawTexture(block, 0, 0, WHITE);
            DrawTexture(block, block_width, 0, WHITE);
            DrawTexture(block, 2 * block_width, 0, WHITE);
            DrawTexture(block, 3 * block_width, 0, WHITE);
            break;
        case J:
            DrawTexture(block, 0, 0, WHITE);
            DrawTexture(block, 0, block_height, WHITE);
            DrawTexture(block, block_width, block_height, WHITE);
            DrawTexture(block, 2 * block_width, block_height, WHITE);
            break;
        case L:
            DrawTexture(block, 2 * block_width, 0, WHITE);
            DrawTexture(block, 0, block_height, WHITE);
            DrawTexture(block, block_width, block_height, WHITE);
            DrawTexture(block, 2 * block_width, block_height, WHITE);
            break;
        case O:
            DrawTexture(block, 0, 0, WHITE);
            DrawTexture(block, block_width, 0, WHITE);
            DrawTexture(block, 0, block_height, WHITE);
            DrawTexture(block, block_width, block_height, WHITE);
            break;
        case S:
            DrawTexture(block, block_width, 0, WHITE);
            DrawTexture(block, 2 * block_width, 0, WHITE);
            DrawTexture(block, 0, block_height, WHITE);
            DrawTexture(block, block_width, block_height, WHITE);
            break;
        case T:
            DrawTexture(block, block_width, 0, WHITE);
            DrawTexture(block, 0, block_height, WHITE);
            DrawTexture(block, block_width, block_height, WHITE);
            DrawTexture(block, 2 * block_width, block_height, WHITE);
            break;
        case Z:
            DrawTexture(block, 0, 0, WHITE);
            DrawTexture(block, block_width, 0, WHITE);
            DrawTexture(block, block_width, block_height, WHITE);
            DrawTexture(block, 2 * block_width, block_height, WHITE);
            break;
    }; 
    EndTextureMode();
    UnloadTexture(block);
    UnloadImage(_block);
    return next_shape;
}