#include <stdlib.h>
#include <renderer.h>
#include <definitions.h>


/// @brief render the wall texture and return it
/// @param wall_img_path path to wall image
/// @param scoreboard_img_path path to scboard image
/// @return the rendered wall texture
RenderTexture2D wall_and_board_renderer(const char* wall_img_path, const char* scoreboard_img_path) {
    Image _wall = LoadImage(wall_img_path);
    Texture2D wall = LoadTextureFromImage(_wall);
    Image _scboard = LoadImage(scoreboard_img_path);
    Texture2D scboard = LoadTextureFromImage(_scboard);
    RenderTexture2D wall_and_scboard_render = LoadRenderTexture(gamew + scoreboard_width, gameh);
    BeginTextureMode(wall_and_scboard_render);
    // draw walls
    for (int i = 0; i <= gameh; i += gameh / wall_height) {
        DrawTexture(wall, 0, i, WHITE);
        DrawTexture(wall, gamew - block_width, i, WHITE);
    }
    // draw scboard
    DrawTexture(scboard, gamew, 0, WHITE);
    EndTextureMode();
    #pragma region cleanup
        UnloadTexture(scboard);
        UnloadImage(_scboard);
        UnloadTexture(wall);
        UnloadImage(_wall);
    #pragma endregion
    return wall_and_scboard_render;
}


RenderTexture2D gen_next_block_texture(tetromino_shapes* out_shape)
{
    // gen random block shape
    const tetromino_shapes next_code = rand() % 7;
    if (out_shape != NULL) *out_shape = next_code;
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