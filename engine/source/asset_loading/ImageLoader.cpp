#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#endif

#include "ImageLoader.h"

Image ImageLoader::load_image(const char* t_path)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixel_buffer = stbi_load(t_path,
		&width,
		&height,
		&channels,
		STBI_rgb_alpha
	);

	return Image{
		static_cast<unsigned int>(width),
		static_cast<unsigned int>(height),
		static_cast<unsigned int>(channels),
		pixel_buffer
	};
}