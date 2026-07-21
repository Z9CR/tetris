#ifndef DEFINATIONS
#define DEFINATIONS

#define block_size 64
#define vertical_cnt 18
#define horizonal_cnt 12

#define game_fps 60
#define gamew (horizonal_cnt * block_size)
#define gameh (vertical_cnt * block_size)
// the `0` here will be replaced into scoreboard's width
// divide by 2 because the original game frame is too big, 
// the physical screen cant display all
#define winw ((gamew + 0) / 2)
#define winh (gameh / 2)
#define wintitle "Tetris"

#endif