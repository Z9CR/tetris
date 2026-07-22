#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <renderer.h>
#include <definitions.h>
#include <tetromino.h>

int main(void)
{
    // init window
    InitWindow(winw, winh, wintitle);
    SetTargetFPS(game_fps);
    // init audio & bgm
    InitAudioDevice();
    const Music bgm = LoadMusicStream("assets/theme.mp3");
    PlayMusicStream(bgm);
    RenderTexture2D walls_and_scoreboard = wall_and_board_renderer("assets/wall.png", "assets/score_board.png");
    while (!WindowShouldClose())
    {
        UpdateMusicStream(bgm);
        BeginDrawing();
        ClearBackground(background_color);
        // draw walls & score board
        {
            Rectangle src = { 0, 0, (float)walls_and_scoreboard.texture.width, -(float)walls_and_scoreboard.texture.height };
            Rectangle dst = { 0, 0, walls_and_scoreboard.texture.width * 0.5f, walls_and_scoreboard.texture.height * 0.5f };
            DrawTexturePro(walls_and_scoreboard.texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
        }
        // gen next block
        RenderTexture2D next_tetromino = gen_next_block_texture();
        // ##########
        //    ####   <--- block'll be generated there(tetromino's leftest block is 4block_width away from left boarder)
        // start tetromino falling loop
        while (true){
            
            Rectangle src = { 4 * scale * block_width, 0, (float)next_tetromino.texture.width, -(float)next_tetromino.texture.height };
            Rectangle dst = { 2 * scale * block_width, 0, next_tetromino.texture.width * 0.5f, next_tetromino.texture.height * 0.5f };
            DrawTexturePro(next_tetromino.texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
        }
        EndDrawing();
        UnloadRenderTexture(next_tetromino);
    }
    #pragma region cleanup
    UnloadRenderTexture(walls_and_scoreboard);
    UnloadMusicStream(bgm);
    CloseAudioDevice();
    CloseWindow();
    #pragma endregion
    return 0;
}