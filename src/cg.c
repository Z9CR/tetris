#include <cg.h>
#include <raylib.h>
#include <definitions.h>

void show_tittle_screen_cg(const char* pic_path, const char* music_path, Font fnt) {
    Music bgm = LoadMusicStream(music_path);
    Image _pic = LoadImage(pic_path);
    Texture2D pic = LoadTextureFromImage(_pic);
    PlayMusicStream(bgm);
    while (!(IsKeyPressed(KEY_ENTER) || WindowShouldClose())) {
        UpdateMusicStream(bgm);
        BeginDrawing();
        ClearBackground(background_color);
        DrawTextureEx(pic, (Vector2){0, 0}, 0.0f, 0.5f, WHITE);
        DrawTextEx(
            fnt,
            "PRESS ENTER",
            (Vector2){.x = 0, .y = 0},
            text_pt,
            0.1f,
            WHITE
        );
        EndDrawing();
    }
    UnloadTexture(pic);
    UnloadImage(_pic);
    UnloadMusicStream(bgm);
}