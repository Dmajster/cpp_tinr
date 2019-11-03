#pragma once
#include <GL/glew.h>

class Texture
{
public:
	explicit Texture(GLenum t_texture_type) : m_texture_type(t_texture_type)
	{
		glCreateTextures(m_texture_type, 1, reinterpret_cast<unsigned int*>(&m_texture_id));
	}

	void bind() const
	{
		glBindTexture(m_texture_type, m_texture_id);
	}

	void unbind() const
	{
		glBindTexture(m_texture_type, 0);
	}

	void set_data(unsigned int t_texture_level, unsigned int t_internal_format, unsigned int t_image_width, unsigned int t_image_height, 
		unsigned int t_border, unsigned int t_format, unsigned int t_pixel_type, void* t_pixel_buffer)
	{
		texture_width = t_image_width;
		texture_height = t_image_height;
		glTexImage2D(m_texture_type, t_texture_level, t_internal_format, t_image_width, 
			t_image_height, t_border, t_format, t_pixel_type, t_pixel_buffer);
	}

	void set_parameter(unsigned int t_parameter, unsigned int t_value) const
	{
		glTexParameteri(m_texture_type, t_parameter, t_value);
	}

	size_t texture_width{0};
	size_t texture_height{0};
	
private:
	size_t m_texture_id{0};
	size_t m_texture_type{0};
};
