#pragma once
#include <cstdlib>

#include "../rendering/Texture.h"
#include "asset_loading/ImageLoader.h"

struct SpriteData
{
	Texture* texture;
	float u_start;
	float v_start;
	float u_end;
	float v_end;
};

class SpriteStrip
{
public:
	SpriteStrip(Texture* t_texture, size_t t_frame_width, size_t t_frame_height, size_t t_frame_count) :
		frame_width(t_frame_width), frame_height(t_frame_height), frame_count(t_frame_count), texture(t_texture) {}

	static SpriteStrip* bind_and_create_sprite_strip(Image* t_image, size_t t_frame_width,
		size_t t_frame_height, size_t t_frame_count)
	{
		auto texture = new Texture(GL_TEXTURE_2D);
		texture->bind();
		texture->set_data(0, GL_RGBA, t_image->width, t_image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, t_image->pixel_buffer);
		texture->set_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		texture->set_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return new SpriteStrip(texture, t_frame_width, t_frame_height, t_frame_count);
	}

	SpriteData* return_frame_by_frame_index(const size_t t_frame_index) const
	{
		const size_t strip_columns = texture->texture_width / frame_width;
		const size_t frame_x = t_frame_index % strip_columns;
		const size_t frame_y = t_frame_index / strip_columns;

		return new SpriteData {
			texture,
			static_cast<float>(frame_x) * frame_width / texture->texture_width,
			static_cast<float>(frame_y) * frame_height / texture->texture_height,
			static_cast<float>(frame_x + 1) * frame_width / texture->texture_width,
			static_cast<float>(frame_y + 1) * frame_height / texture->texture_height,
		};
	}

	size_t frame_width{};
	size_t frame_height{};
	size_t frame_count{};
	Texture* texture;
};
