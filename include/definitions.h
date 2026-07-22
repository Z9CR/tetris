#ifndef DEFINITIONS
#define DEFINITIONS
#define block_width 64
#define block_height 64
#define wall_height 48
#define scoreboard_width 440
#define vertical_cnt 18
#define horizonal_cnt 12
#define background_color (Color){0xC6, 0xCF, 0xA1, 0xFF}

#define game_fps 60
#define tetromino_falling_speed 1.2 // ? blocks per second
#define tetromino_translating_delay 0.05 // delay ? secs after a translation
#define gamew (horizonal_cnt * block_width)
#define gameh (vertical_cnt * block_width)
#define game_scale 0.5f
// divide by 2 because the original game frame is too big, 
// the physical screen cant display all
#define winw ((gamew + scoreboard_width) / 2)
#define winh (gameh * game_scale)
#define wintitle "Tetris"
#endif