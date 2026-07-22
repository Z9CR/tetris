#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <renderer.h>
#include <definitions.h>
#include <tetromino.h>

#define MAX_LOCKED 1024

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

    // locked tetromino list
    locked_tetromino locked[MAX_LOCKED];
    int locked_count = 0;

    // init current tetromino
    active_tetromino current;
    current.pos_x = (float)(4 * block_width);
    current.pos_y = 0.0f;
    current.fall_timer = 0.0f;
    current.rotation = 0;
    RenderTexture2D current_rt = gen_next_block_texture(&current.shape);

    while (!WindowShouldClose())
    {
        // ── INPUT ──────────────────────────────────
        int cur_cells[4][2];
        get_tetromino_cells(current.shape, current.rotation, cur_cells);

        // A: try move left
        if (IsKeyPressed(KEY_A) || (IsKeyDown(KEY_A) && current.fall_timer > 0.1f)) {
            float new_x = clamp_within_walls(current.pos_x - (float)block_width,
                                             current.shape, current.rotation);
            int blocked = 0;
            for (int ci = 0; ci < 4 && !blocked; ci++) {
                float cx = new_x + cur_cells[ci][0] * (float)block_width;
                float cy = current.pos_y + cur_cells[ci][1] * (float)block_height;
                for (int i = 0; i < locked_count && !blocked; i++) {
                    int lt_cells[4][2];
                    get_tetromino_cells(locked[i].shape, locked[i].rotation, lt_cells);
                    for (int lj = 0; lj < 4; lj++) {
                        float lx = locked[i].pos_x + lt_cells[lj][0] * (float)block_width;
                        float ly = locked[i].pos_y + lt_cells[lj][1] * (float)block_height;
                        if (cx == lx && cy == ly) { blocked = 1; break; }
                    }
                }
            }
            if (!blocked) current.pos_x = new_x;
        }
        // D: try move right
        if (IsKeyPressed(KEY_D) || (IsKeyDown(KEY_D) && current.fall_timer > 0.1f)) {
            float new_x = clamp_within_walls(current.pos_x + (float)block_width,
                                             current.shape, current.rotation);
            int blocked = 0;
            for (int ci = 0; ci < 4 && !blocked; ci++) {
                float cx = new_x + cur_cells[ci][0] * (float)block_width;
                float cy = current.pos_y + cur_cells[ci][1] * (float)block_height;
                for (int i = 0; i < locked_count && !blocked; i++) {
                    int lt_cells[4][2];
                    get_tetromino_cells(locked[i].shape, locked[i].rotation, lt_cells);
                    for (int lj = 0; lj < 4; lj++) {
                        float lx = locked[i].pos_x + lt_cells[lj][0] * (float)block_width;
                        float ly = locked[i].pos_y + lt_cells[lj][1] * (float)block_height;
                        if (cx == lx && cy == ly) { blocked = 1; break; }
                    }
                }
            }
            if (!blocked) current.pos_x = new_x;
        }
        // Z: rotate CCW
        if (IsKeyPressed(KEY_Z)) {
            int new_rot = (current.rotation + 3) % 4;
            float new_x = clamp_within_walls(current.pos_x, current.shape, new_rot);
            int cells_new[4][2];
            get_tetromino_cells(current.shape, new_rot, cells_new);
            int blocked = 0;
            for (int ci = 0; ci < 4 && !blocked; ci++) {
                float cx = new_x + cells_new[ci][0] * (float)block_width;
                float cy = current.pos_y + cells_new[ci][1] * (float)block_height;
                for (int i = 0; i < locked_count && !blocked; i++) {
                    int lt_cells[4][2];
                    get_tetromino_cells(locked[i].shape, locked[i].rotation, lt_cells);
                    for (int lj = 0; lj < 4; lj++) {
                        float lx = locked[i].pos_x + lt_cells[lj][0] * (float)block_width;
                        float ly = locked[i].pos_y + lt_cells[lj][1] * (float)block_height;
                        if (cx == lx && cy == ly) { blocked = 1; break; }
                    }
                }
            }
            if (!blocked) {
                UnloadRenderTexture(current_rt);
                current.rotation = new_rot;
                current.pos_x = new_x;
                current_rt = rebuild_tetromino_rt(current.shape, current.rotation);
            }
        }
        // C: rotate CW
        if (IsKeyPressed(KEY_C)) {
            int new_rot = (current.rotation + 1) % 4;
            float new_x = clamp_within_walls(current.pos_x, current.shape, new_rot);
            int cells_new[4][2];
            get_tetromino_cells(current.shape, new_rot, cells_new);
            int blocked = 0;
            for (int ci = 0; ci < 4 && !blocked; ci++) {
                float cx = new_x + cells_new[ci][0] * (float)block_width;
                float cy = current.pos_y + cells_new[ci][1] * (float)block_height;
                for (int i = 0; i < locked_count && !blocked; i++) {
                    int lt_cells[4][2];
                    get_tetromino_cells(locked[i].shape, locked[i].rotation, lt_cells);
                    for (int lj = 0; lj < 4; lj++) {
                        float lx = locked[i].pos_x + lt_cells[lj][0] * (float)block_width;
                        float ly = locked[i].pos_y + lt_cells[lj][1] * (float)block_height;
                        if (cx == lx && cy == ly) { blocked = 1; break; }
                    }
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
            int tetro_w = get_tetromino_width(current.shape, current.rotation);
            int tetro_h = get_tetromino_height(current.shape, current.rotation);

            int collided = 0;

            // get current tetromino cells at next_y
            int cur_cells[4][2];
            get_tetromino_cells(current.shape, current.rotation, cur_cells);

            // check locked-tetromino collision (per-cell)
            for (int i = 0; i < locked_count && !collided; i++) {
                locked_tetromino *lt = &locked[i];
                int lt_cells[4][2];
                get_tetromino_cells(lt->shape, lt->rotation, lt_cells);

                for (int ci = 0; ci < 4; ci++) {
                    float cx = current.pos_x + cur_cells[ci][0] * (float)block_width;
                    float cy = next_y        + cur_cells[ci][1] * (float)block_height;
                    for (int lj = 0; lj < 4; lj++) {
                        float lx = lt->pos_x + lt_cells[lj][0] * (float)block_width;
                        float ly = lt->pos_y + lt_cells[lj][1] * (float)block_height;
                        if (cx == lx && cy == ly) {
                            collided = 1;
                            break;
                        }
                    }
                    if (collided) break;
                }
            }

            // check bottom boundary collision
            if (next_y + tetro_h > (float)gameh) {
                collided = 1;
            }

            if (collided) {
                // lock current tetromino
                if (locked_count < MAX_LOCKED) {
                    locked[locked_count].rt       = current_rt;
                    locked[locked_count].shape    = current.shape;
                    locked[locked_count].rotation = current.rotation;
                    locked[locked_count].pos_x    = current.pos_x;
                    locked[locked_count].pos_y    = current.pos_y;
                    locked_count++;
                }
                // spawn new tetromino
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

        // ── RENDER ─────────────────────────────────
        UpdateMusicStream(bgm);
        BeginDrawing();
        ClearBackground(background_color);
        {
            // walls & scoreboard
            Rectangle src = { 0, 0, (float)walls_and_scoreboard.texture.width, -(float)walls_and_scoreboard.texture.height };
            Rectangle dst = { 0, 0, walls_and_scoreboard.texture.width * game_scale, walls_and_scoreboard.texture.height * game_scale };
            DrawTexturePro(walls_and_scoreboard.texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
        }
        {
            // render all locked tetrominos
            for (int i = 0; i < locked_count; i++) {
                locked_tetromino *lt = &locked[i];
                Rectangle src = { 0, 0, (float)lt->rt.texture.width, -(float)lt->rt.texture.height };
                Rectangle dst = {
                    lt->pos_x * game_scale,
                    lt->pos_y * game_scale,
                    lt->rt.texture.width * game_scale,
                    lt->rt.texture.height * game_scale
                };
                DrawTexturePro(lt->rt.texture, src, dst, (Vector2){0, 0}, 0.0f, WHITE);
            }
        }
        {
            // render current falling tetromino
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
    for (int i = 0; i < locked_count; i++) {
        UnloadRenderTexture(locked[i].rt);
    }
    UnloadRenderTexture(walls_and_scoreboard);
    UnloadMusicStream(bgm);
    CloseAudioDevice();
    CloseWindow();
    #pragma endregion
    return 0;
}