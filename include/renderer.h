#ifndef RENDERER
#define RENDERER
#include <raylib.h>

/// @brief render the wall texture and return it
/// @param wall_img_path path to wall image
/// @return the rendered wall texture
RenderTexture2D wall_renderer(const char *wall_img_path);

/// @brief Gen next tetromino's RenderTexture2D
/// @return next tetromino's RenderTexture2D
RenderTexture2D gen_next_block_texture(void);

#endif
