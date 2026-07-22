#ifndef RENDERER
#define RENDERER
#include <raylib.h>
#include <tetromino.h>

/// @brief render the wall texture and return it
/// @param wall_img_path path to wall image
/// @param scoreboard_img_path path to scboard image
/// @return the rendered wall texture
RenderTexture2D wall_and_board_renderer(const char *wall_img_path, const char* scoreboard_img_path);

/// @brief Gen next tetromino's RenderTexture2D
/// @param[out] out_shape the shape code of the generated tetromino
/// @return next tetromino's RenderTexture2D
RenderTexture2D gen_next_block_texture(tetromino_shapes* out_shape);

#endif
