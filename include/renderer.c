#include <definitions.h>
#include <tetromino.h>
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
    #pragma region cleanup
        UnloadTexture(wall);
        UnloadImage(_wall);
    #pragma endregion
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
    draw_tetromino_code(next_code, &next_shape, &block);
    #pragma region cleanup
    UnloadTexture(block);
    UnloadImage(_block);
    #pragma endregion
    return next_shape;
}