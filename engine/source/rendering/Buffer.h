#pragma once
#include <GL/glew.h>

class Buffer
{
public:
	explicit Buffer(const GLenum t_type) : buffer_type(t_type)
	{
		glCreateBuffers(1, &buffer_id);
	}

	void bind() const
	{
		glBindBuffer(buffer_type, buffer_id);
	}

	void unbind() const
	{
		glBindBuffer(buffer_type, 0);
	}

	void set_data(const size_t size_in_bytes, const void* data, const GLenum usage = GL_STATIC_DRAW)
	{
		buffer_size = size_in_bytes;
		buffer_usage = usage;

		glBufferData(buffer_type, buffer_size, data, buffer_usage);
	}

	GLuint buffer_id{ 0 };
	GLenum buffer_type{ 0 };
	GLenum buffer_usage{ 0 };
	size_t buffer_size{ 0 };
};
