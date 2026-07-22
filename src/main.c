#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <renderer.h>
#include <definitions.h>
#include <tetromino.h>
#include <cg.h>

#define MAX_LOCKED (vertical_cnt * horizonal_cnt)
static unsigned int lines = 0;
static unsigned int score = 0;

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
    InitWindow((int)winw, (int)winh, wintitle);
    SetTargetFPS(game_fps);
    InitAudioDevice();

    Font fnt = LoadFont("assets/font.ttf");
    Texture2D block_tex[7];
    for (int i = 0; i < 7; i++) {
        Image img = LoadImage(TextFormat("assets/blocks/%d.png", i));
        block_tex[i] = LoadTextureFromImage(img);
        UnloadImage(img);
    }

    // ── outer loop: one complete game session per iteration ──
    while (!WindowShouldClose())
    {
        // reset game state
        lines = 0;
        score = 0;
        int grid[vertical_cnt][horizonal_cnt] = {0};
        locked_cell locked[MAX_LOCKED];
        int locked_count = 0;

        const Music bgm = LoadMusicStream("assets/theme.mp3");
        RenderTexture2D walls_and_scoreboard = wall_and_board_renderer("assets/wall.png", "assets/score_board.png");

        active_tetromino current;
        current.pos_x = (float)(4 * block_width);
        current.pos_y = 0.0f;
        current.fall_timer = 0.0f;
        current.translate_timer = (float)tetromino_translating_delay;
        current.rotation = 0;
        RenderTexture2D current_rt = gen_next_block_texture(&current.shape);

        tetromino_shapes next_shape;
        RenderTexture2D next_rt = gen_next_block_texture(&next_shape);
        float preview_x = (float)(gamew + 0.28f * scoreboard_width);
        float preview_y = (float)(gameh - 0.55f * scoreboard_width);

        show_tittle_screen_cg("assets/title_screen.png", "assets/title_screen.mp3", fnt);
        PlayMusicStream(bgm);

        int playing = 1;
        while (playing && !WindowShouldClose())
        {
            // ── INPUT ──────────────────────────────────
            current.translate_timer += GetFrameTime();
            int cur_cells[4][2];
            get_tetromino_cells(current.shape, current.rotation, cur_cells);

            if (IsKeyDown(KEY_A) && current.translate_timer >= (float)tetromino_translating_delay) {
                float new_x = clamp_within_walls(current.pos_x - (float)block_width,
                                                 current.shape, current.rotation);
                int blocked = 0;
                for (int ci = 0; ci < 4 && !blocked; ci++) {
                    int gx = (int)(new_x / block_width) + cur_cells[ci][0];
                    int gy = (int)(current.pos_y / block_height) + cur_cells[ci][1];
                    if (gx >= 1 && gx < horizonal_cnt - 1 && gy >= 0 && gy < vertical_cnt) {
                        if (grid[gy][gx]) blocked = 1;
                    }
                }
                if (!blocked) { current.pos_x = new_x; current.translate_timer = 0.0f; }
            }
            if (IsKeyDown(KEY_D) && current.translate_timer >= (float)tetromino_translating_delay) {
                float new_x = clamp_within_walls(current.pos_x + (float)block_width,
                                                 current.shape, current.rotation);
                int blocked = 0;
                for (int ci = 0; ci < 4 && !blocked; ci++) {
                    int gx = (int)(new_x / block_width) + cur_cells[ci][0];
                    int gy = (int)(current.pos_y / block_height) + cur_cells[ci][1];
                    if (gx >= 1 && gx < horizonal_cnt - 1 && gy >= 0 && gy < vertical_cnt) {
                        if (grid[gy][gx]) blocked = 1;
                    }
                }
                if (!blocked) { current.pos_x = new_x; current.translate_timer = 0.0f; }
            }
            if (IsKeyPressed(KEY_Z)) {
                int new_rot = (current.rotation + 3) % 4;
                float new_x = clamp_within_walls(current.pos_x, current.shape, new_rot);
                int cells_new[4][2];
                get_tetromino_cells(current.shape, new_rot, cells_new);
                int blocked = 0;
                for (int ci = 0; ci < 4 && !blocked; ci++) {
                    int gx = (int)(new_x / block_width) + cells_new[ci][0];
                    int gy = (int)(current.pos_y / block_height) + cells_new[ci][1];
                    if (gx >= 1 && gx < horizonal_cnt - 1 && gy >= 0 && gy < vertical_cnt) {
                        if (grid[gy][gx]) blocked = 1;
                    }
                }
                if (!blocked) {
                    UnloadRenderTexture(current_rt);
                    current.rotation = new_rot;
                    current.pos_x = new_x;
                    current_rt = rebuild_tetromino_rt(current.shape, current.rotation);
                }
            }
            if (IsKeyPressed(KEY_C)) {
                int new_rot = (current.rotation + 1) % 4;
                float new_x = clamp_within_walls(current.pos_x, current.shape, new_rot);
                int cells_new[4][2];
                get_tetromino_cells(current.shape, new_rot, cells_new);
                int blocked = 0;
                for (int ci = 0; ci < 4 && !blocked; ci++) {
                    int gx = (int)(new_x / block_width) + cells_new[ci][0];
                    int gy = (int)(current.pos_y / block_height) + cells_new[ci][1];
                    if (gx >= 1 && gx < horizonal_cnt - 1 && gy >= 0 && gy < vertical_cnt) {
                        if (grid[gy][gx]) blocked = 1;
                    }
                }
                if (!blocked) {
                    UnloadRenderTexture(current_rt);
                    current.rotation = new_rot;
                    current.pos_x = new_x;
                    current_rt = rebuild_tetromino_rt(current.shape, current.rotation);
                }
            }

            // ── FALLING ────────────────────────────────
            current.fall_timer += GetFrameTime();
            float fall_speed_mult = IsKeyDown(KEY_S) ? 20.0f : 1.0f;
            float fall_interval = 1.0f / (tetromino_falling_speed * fall_speed_mult);

            if (current.fall_timer >= fall_interval) {
                float next_y = current.pos_y + (float)block_height;
                int cur_cells_fall[4][2];
                get_tetromino_cells(current.shape, current.rotation, cur_cells_fall);

                int collided = 0;
                for (int ci = 0; ci < 4 && !collided; ci++) {
                    int gx = (int)(current.pos_x / block_width) + cur_cells_fall[ci][0];
                    int gy = (int)(next_y / block_height) + cur_cells_fall[ci][1];
                    if (gy >= vertical_cnt || grid[gy][gx]) collided = 1;
                }

                if (collided) {
                    int cells_lock[4][2];
                    get_tetromino_cells(current.shape, current.rotation, cells_lock);
                    for (int ci = 0; ci < 4; ci++) {
                        int gx = (int)(current.pos_x / block_width) + cells_lock[ci][0];
                        int gy = (int)(current.pos_y / block_height) + cells_lock[ci][1];
                        grid[gy][gx] = 1;
                        locked[locked_count].shape    = current.shape;
                        locked[locked_count].grid_col = gx;
                        locked[locked_count].grid_row = gy;
                        locked_count++;
                    }

                    int lines_cleared = 0;
                    for (int row = 0; row < vertical_cnt; row++) {
                        int full = 1;
                        for (int col = 1; col < horizonal_cnt - 1; col++) {
                            if (!grid[row][col]) { full = 0; break; }
                        }
                        if (full) {
                            lines++;
                            lines_cleared++;
                            for (int i = locked_count - 1; i >= 0; i--) {
                                if (locked[i].grid_row == row) {
                                    locked[i] = locked[locked_count - 1];
                                    locked_count--;
                                }
                            }
                            for (int r = row; r > 0; r--) {
                                for (int c = 0; c < horizonal_cnt; c++) {
                                    grid[r][c] = grid[r - 1][c];
                                }
                            }
                            for (int c = 0; c < horizonal_cnt; c++) grid[0][c] = 0;
                            for (int i = 0; i < locked_count; i++) {
                                if (locked[i].grid_row < row) locked[i].grid_row++;
                            }
                            row--;
                        }
                    }

                    switch (lines_cleared) {
                        case 1: score += 40;   break;
                        case 2: score += 100;  break;
                        case 3: score += 300;  break;
                        case 4: score += 1200; break;
                        default: break;
                    }

                    current.shape = next_shape;
                    current.pos_x = (float)(4 * block_width);
                    current.pos_y = 0.0f;
                    current.fall_timer = 0.0f;
                    current.rotation = 0;
                    UnloadRenderTexture(current_rt);
                    current_rt = rebuild_tetromino_rt(current.shape, 0);
                    UnloadRenderTexture(next_rt);
                    next_rt = gen_next_block_texture(&next_shape);

                    // check game over
                    {
                        int spawn_cells[4][2];
                        get_tetromino_cells(current.shape, 0, spawn_cells);
                        int dead = 0;
                        for (int ci = 0; ci < 4 && !dead; ci++) {
                            int gx = (int)(current.pos_x / block_width) + spawn_cells[ci][0];
                            int gy = (int)(current.pos_y / block_height) + spawn_cells[ci][1];
                            if (grid[gy][gx]) dead = 1;
                        }
                        if (dead) {
                            // show_gameover_screen_cg("assets/blocks/5.png", bgm);
                            // wait for ENTER to restart
                            while (!IsKeyPressed(KEY_ENTER) && !WindowShouldClose()) {
                                BeginDrawing();
                                StopMusicStream(bgm);
                                // outline
                                for (int dy = -1; dy <= 1; dy++) {
                                    for (int dx = -1; dx <= 1; dx++) {
                                        if (dx == 0 && dy == 0) continue;
                                        DrawTextEx(
                                            fnt, "GAME OVER\nPRESS ENTER",
                                            (Vector2){
                                                .x = (winw - MeasureTextEx(fnt, "PRESS ENTER", text_pt, 0.1f).x) / 2.0f + dx * 1.0f,
                                                .y = 0.45f * winh + dy * 1.0f
                                            },
                                            text_pt, 0.1f, (Color){255, 255, 255, 200});
                                    }
                                }
                                // fill text
                                DrawTextEx(
                                    fnt, "GAME OVER\nPRESS ENTER",
                                    (Vector2){
                                        .x = (winw - MeasureTextEx(fnt, "PRESS ENTER", text_pt, 0.1f).x) / 2.0f,
                                        .y = 0.45f * winh
                                    },
                                    text_pt, 0.1f,
                                    (Color){
                                        .a = 0xFF,
                                        .r = text_color.r + 10,
                                        .g = text_color.g + 10,
                                        .b = text_color.b + 30
                                    });
                                
                                EndDrawing();
                            }
                            playing = 0;
                        }
                    }
                } else {
                    current.pos_y = next_y;
                    current.fall_timer -= fall_interval;
                }
            }

            // ── RENDER ─────────────────────────────────
            UpdateMusicStream(bgm);
            BeginDrawing();
            ClearBackground(background_color);
            {
                Rectangle src = { 0, 0, (float)walls_and_scoreboard.texture.width, -(float)walls_and_scoreboard.texture.height };
                Rectangle dst = { 0, 0, walls_and_scoreboard.texture.width * game_scale, walls_and_scoreboard.texture.height * game_scale };
                DrawTexturePro(walls_and_scoreboard.texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
            }
            for (int i = 0; i < locked_count; i++) {
                locked_cell *lc = &locked[i];
                float lx = (float)(lc->grid_col * block_width);
                float ly = (float)(lc->grid_row * block_height);
                DrawTextureEx(block_tex[lc->shape], (Vector2){lx * game_scale, ly * game_scale}, 0.0f, game_scale, WHITE);
            }
            {
                Rectangle src = { 0, 0, (float)current_rt.texture.width, -(float)current_rt.texture.height };
                Rectangle dst = {
                    current.pos_x * game_scale, current.pos_y * game_scale,
                    current_rt.texture.width * game_scale, current_rt.texture.height * game_scale
                };
                DrawTexturePro(current_rt.texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
            }
            {
                Rectangle src = { 0, 0, (float)next_rt.texture.width, -(float)next_rt.texture.height };
                Rectangle dst = {
                    preview_x * game_scale, preview_y * game_scale,
                    next_rt.texture.width * game_scale, next_rt.texture.height * game_scale
                };
                DrawTexturePro(next_rt.texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
            }
            DrawTextEx(fnt, TextFormat("%u", score),
                       (Vector2){ game_scale * (64.0f/440.0f * (float)scoreboard_width + (float)gamew),
                                  game_scale * (175.0f/1152.0f * (float)gameh) },
                       text_pt, 0.1f, text_color);
            DrawTextEx(fnt, TextFormat("%u", 1u),
                       (Vector2){ game_scale * (64.0f/440.0f * (float)scoreboard_width + (float)gamew),
                                  game_scale * (437.0f/1152.0f * (float)gameh) },
                       text_pt, 0.1f, text_color);
            DrawTextEx(fnt, TextFormat("%u", lines),
                       (Vector2){ game_scale * (64.0f/440.0f * (float)scoreboard_width + (float)gamew),
                                  game_scale * (620.0f/1152.0f * (float)gameh) },
                       text_pt, 0.1f, text_color);
            EndDrawing();
        }

        #pragma region session_cleanup
        UnloadRenderTexture(current_rt);
        UnloadRenderTexture(next_rt);
        UnloadRenderTexture(walls_and_scoreboard);
        UnloadMusicStream(bgm);
        #pragma endregion
    }

    #pragma region cleanup
    UnloadFont(fnt);
    for (int i = 0; i < 7; i++) UnloadTexture(block_tex[i]);
    CloseAudioDevice();
    CloseWindow();
    #pragma endregion
    return 0;
}