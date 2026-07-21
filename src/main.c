#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <renderer.h>
#include <definitions.h>
#include <utils.h>

int main(void)
{
    // init window
    InitWindow(winw, winh, wintitle);
    SetTargetFPS(game_fps);
    // init audio & bgm
    InitAudioDevice();
    const Music bgm = LoadMusicStream("assets/theme.mp3");
    PlayMusicStream(bgm);
    RenderTexture2D walls = wall_renderer("assets/wall.png");
    while (!WindowShouldClose())
    {
        UpdateMusicStream(bgm);
        BeginDrawing();
        // draw walls
        DrawTextureEx(walls.texture, (Vector2){0, 0}, 0.0f, 0.5f, WHITE);
        // gen next block
        RenderTexture2D next_tetromino = gen_next_block_texture();
        DrawTextureEx(next_tetromino.texture, (Vector2){0, 0}, 0.0f, 0.5f, WHITE);
        ClearBackground(background_color);
        EndDrawing();
        UnloadRenderTexture(next_tetromino);
    }
    UnloadRenderTexture(walls);
    UnloadMusicStream(bgm);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}