#include <raylib.h>
#include <cg.h>
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
        DrawTexturePro(
            pic,
            (Rectangle){ 0, 0, (float)pic.width, (float)pic.height },
            (Rectangle){ 0, 0, (float)winw, (float)winh },
            (Vector2){ 0, 0 }, 0.0f, WHITE
        );
        DrawTextEx(
            fnt,
            "PRESS ENTER",
            (Vector2){
                .x = (winw - MeasureTextEx(fnt, "PRESS ENTER", text_pt, 0.1f).x) / 2.0f,
                .y = 0.8f * winh
            },
            text_pt,
            0.1f,
            text_color
        );
        EndDrawing();
    }
    UnloadTexture(pic);
    UnloadImage(_pic);
    UnloadMusicStream(bgm);
}