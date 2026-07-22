#ifndef RENDERER
#define RENDERER
#include <raylib.h>
#include <tetromino.h>

/// @brief render the wall texture and return it
/// @param wall_img_path path to wall image
/// @param scoreboard_img_path path to scboard image
/// @return the rendered wall texture
RenderTexture2D wall_and_board_renderer(const char *wall_img_path, const char* scoreboard_img_path);

/// @brief Gen next tetromino's RenderTexture2D at rotation 0
/// @param[out] out_shape the shape code of the generated tetromino
/// @return next tetromino's RenderTexture2D
RenderTexture2D gen_next_block_texture(tetromino_shapes* out_shape);

/// @brief Rebuild a tetromino's RenderTexture2D for a given shape+rotation
/// @param shape the tetromino shape
/// @param rotation 0-3 (CW 90° steps)
/// @return new RenderTexture2D (caller must Unload old one first)
RenderTexture2D rebuild_tetromino_rt(tetromino_shapes shape, int rotation);

#endif
