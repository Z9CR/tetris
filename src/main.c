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

    // init current tetromino (spawn at top-center of game area)
    active_tetromino current;
    current.pos_x = (float)(4 * block_width);  // ~center of 12-wide board
    current.pos_y = 0.0f;
    current.fall_timer = 0.0f;
    RenderTexture2D current_rt = gen_next_block_texture(&current.shape);

    while (!WindowShouldClose())
    {
        // time-accumulated falling
        current.fall_timer += GetFrameTime();
        float fall_interval = 1.0f / tetromino_falling_speed;  // seconds per block drop
        if (current.fall_timer >= fall_interval) {
            // check bottom collision
            float next_y = current.pos_y + (float)block_height;
            int tetro_height = get_tetromino_height(current.shape);
            if (next_y + tetro_height > (float)gameh) {
                // bottom reached — lock and spawn new tetromino
                UnloadRenderTexture(current_rt);
                current.pos_x = (float)(4 * block_width);
                current.pos_y = 0.0f;
                current.fall_timer = 0.0f;
                current_rt = gen_next_block_texture(&current.shape);
            } else {
                current.pos_y = next_y;
                current.fall_timer -= fall_interval;  // keep remainder for smooth timing
            }
        }

        UpdateMusicStream(bgm);
        BeginDrawing();
        ClearBackground(background_color);
        {
            // draw walls & scoreboard
            Rectangle src = { 0, 0, (float)walls_and_scoreboard.texture.width, -(float)walls_and_scoreboard.texture.height };
            Rectangle dst = { 0, 0, walls_and_scoreboard.texture.width * game_scale, walls_and_scoreboard.texture.height * game_scale };
            DrawTexturePro(walls_and_scoreboard.texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
        }
        {
            // draw current falling tetromino (with RenderTexture Y-flip and scale)
            Rectangle src = { 0, 0, (float)current_rt.texture.width, -(float)current_rt.texture.height };
            Rectangle dst = {
                current.pos_x * game_scale,
                current.pos_y * game_scale,
                current_rt.texture.width * game_scale,
                current_rt.texture.height * game_scale
            };
            DrawTexturePro(current_rt.texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
        }
        EndDrawing();
    }

    #pragma region cleanup
    UnloadRenderTexture(current_rt);
    UnloadRenderTexture(walls_and_scoreboard);
    UnloadMusicStream(bgm);
    CloseAudioDevice();
    CloseWindow();
    #pragma endregion
    return 0;
}