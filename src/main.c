#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <renderer.h>
#include <definitions.h>
#include <tetromino.h>

/// Clamp pos_x so the tetromino fits between walls
static float clamp_within_walls(float x, tetromino_shapes shape, int rotation) {
    int w = get_tetromino_width(shape, rotation);
    float left_limit  = (float)block_width;
    float right_limit = (float)(gamew - block_width);
    if (x < left_limit) return left_limit;
    if (x + w > right_limit) return right_limit - (float)w;
    return x;
}

int main(void)
{
    // init window
    InitWindow((int)winw, (int)winh, wintitle);
    SetTargetFPS(game_fps);
    // init audio & bgm
    InitAudioDevice();
    const Music bgm = LoadMusicStream("assets/theme.mp3");
    PlayMusicStream(bgm);
    RenderTexture2D walls_and_scoreboard = wall_and_board_renderer("assets/wall.png", "assets/score_board.png");

    // init current tetromino
    active_tetromino current;
    current.pos_x = (float)(4 * block_width);
    current.pos_y = 0.0f;
    current.fall_timer = 0.0f;
    current.rotation = 0;
    RenderTexture2D current_rt = gen_next_block_texture(&current.shape);

    while (!WindowShouldClose())
    {
        // INPUT
        // A / D: left / right
        if (IsKeyPressed(KEY_A) || (IsKeyDown(KEY_A) && current.fall_timer > 0.1f)) {
            current.pos_x = clamp_within_walls(current.pos_x - (float)block_width,
                                               current.shape, current.rotation);
        }
        if (IsKeyPressed(KEY_D) || (IsKeyDown(KEY_D) && current.fall_timer > 0.1f)) {
            current.pos_x = clamp_within_walls(current.pos_x + (float)block_width,
                                               current.shape, current.rotation);
        }
        // Z: rotate CCW, C: rotate CW (only for O rotation does nothing)
        if (IsKeyPressed(KEY_Z)) {
            int new_rot = (current.rotation + 3) % 4;
            UnloadRenderTexture(current_rt);
            current.rotation = new_rot;
            current_rt = rebuild_tetromino_rt(current.shape, current.rotation);
            current.pos_x = clamp_within_walls(current.pos_x, current.shape, current.rotation);
        }
        if (IsKeyPressed(KEY_C)) {
            int new_rot = (current.rotation + 1) % 4;
            UnloadRenderTexture(current_rt);
            current.rotation = new_rot;
            current_rt = rebuild_tetromino_rt(current.shape, current.rotation);
            current.pos_x = clamp_within_walls(current.pos_x, current.shape, current.rotation);
        }

        // FALLING
        current.fall_timer += GetFrameTime();
        float fall_speed_mult = IsKeyDown(KEY_S) ? 20.0f : 1.0f;
        float fall_interval = 1.0f / (tetromino_falling_speed * fall_speed_mult);

        if (current.fall_timer >= fall_interval) {
            float next_y = current.pos_y + (float)block_height;
            int tetro_h = get_tetromino_height(current.shape, current.rotation);
            if (next_y + tetro_h > (float)gameh) {
                // bottom reached — lock and spawn new tetromino
                UnloadRenderTexture(current_rt);
                current.shape = 0;  // will be set by gen_next
                current.pos_x = (float)(4 * block_width);
                current.pos_y = 0.0f;
                current.fall_timer = 0.0f;
                current.rotation = 0;
                current_rt = gen_next_block_texture(&current.shape);
            } else {
                current.pos_y = next_y;
                current.fall_timer -= fall_interval;
            }
        }

        // RENDER
        UpdateMusicStream(bgm);
        BeginDrawing();
        ClearBackground(background_color);
        {
            Rectangle src = { 0, 0, (float)walls_and_scoreboard.texture.width, -(float)walls_and_scoreboard.texture.height };
            Rectangle dst = { 0, 0, walls_and_scoreboard.texture.width * game_scale, walls_and_scoreboard.texture.height * game_scale };
            DrawTexturePro(walls_and_scoreboard.texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
        }
        {
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