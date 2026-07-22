#ifndef DEFINITIONS
#define DEFINITIONS
#define block_width 64
#define block_height 64
#define wall_height 48
#define scoreboard_width 512 // fit to 10(1280):9(1152)
#define vertical_cnt 18
#define horizonal_cnt 12
#define background_color (Color){0xC6, 0xCF, 0xA1, 0xFF}
#define text_color (Color){0x40, 0x42, 0x43, 0xFF}
#define text_pt (52.0f * game_scale / 0.5f)

#define game_fps 60
#define tetromino_falling_speed 1.2 // ? blocks per second
#define tetromino_translating_delay 0.05 // delay ? secs after a translation
#define gamew (horizonal_cnt * block_width)
#define gameh (vertical_cnt * block_width)
#define game_scale 0.8f
// divide by 2 because the original game frame is too big, 
// the physical screen cant display all
#define winw (game_scale * (gamew + scoreboard_width))
#define winh (gameh * game_scale)
#define wintitle "Tetris"

#ifdef _WIN32
    __declspec(dllimport) void __stdcall Sleep(unsigned long ms);
    #define sleep_ms(ms) Sleep((unsigned long)(ms))
#else
    #include <time.h>
    #define sleep_ms(ms) do { \
        struct timespec ts = { (ms) / 1000, ((ms) % 1000) * 1000000L }; \
        nanosleep(&ts, NULL); \
    } while(0)
#endif

#endif