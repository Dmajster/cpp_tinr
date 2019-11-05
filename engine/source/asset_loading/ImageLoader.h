#pragma once
#include <memory>

struct Image
{
	unsigned int width;
	unsigned int height;
	unsigned int channels;
	void* pixel_buffer;
};

class ImageLoader
{
public:
	static Image load_image(const char* t_path);
};
