#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <defines.h>

int main (void) {
    InitWindow(winw, winh, wintitle);
    SetTargetFPS(game_fps);
    while(!WindowShouldClose()) {
        BeginDrawing();
        
        ClearBackground(BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}