#ifndef DEFINITIONS
#define DEFINITIONS
#define block_width 64
#define block_height 64
#define wall_height 48
#define vertical_cnt 18
#define horizonal_cnt 12
#define background_color (Color){0xC6, 0xCF, 0xA1, 0xFF}

#define game_fps 60
#define gamew (horizonal_cnt * block_width)
#define gameh (vertical_cnt * block_width)
// the `0` here will be replaced into scoreboard's width
// divide by 2 because the original game frame is too big, 
// the physical screen cant display all
#define winw ((gamew + 0) / 2)
#define winh (gameh / 2)
#define wintitle "Tetris"
#endif