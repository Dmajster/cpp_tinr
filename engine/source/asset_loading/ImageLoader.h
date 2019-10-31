#pragma once

struct Image
{
	unsigned int width;
	unsigned int height;
	unsigned int channels;
	void* pixel_buffer;

	~Image() = default;
};

class ImageLoader
{
public:
	static Image loadImage(const char* t_path);
};
