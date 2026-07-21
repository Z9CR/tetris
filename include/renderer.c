#include <renderer.h>
#include <definitions.h>

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
