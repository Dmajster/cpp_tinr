#pragma once
#include <GL/glew.h>

class Texture
{
public:
	Texture(GLenum t_texture_type)
	{
		m_texture_type = t_texture_type;
		glCreateTextures(m_texture_type, 1, &m_texture_id);
	}

private:
	unsigned int m_texture_id;
	unsigned int m_texture_type;
};
