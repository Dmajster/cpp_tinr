#pragma once
#include <GL/glew.h>
#include <vector>

class Shader
{
public:
	explicit Shader(const size_t t_shader_type) : shader_id(glCreateShader(t_shader_type)), shader_type(t_shader_type)
	{
	}

	~Shader()
	{
		glDeleteShader(shader_id);
	}

	void compile(const std::string& t_shader_source) const
	{
		auto shader_source = t_shader_source.c_str();
		
		glShaderSource(shader_id, 1, &shader_source, nullptr);
		glCompileShader(shader_id);

		int compiled;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			auto max_length = 0;
			glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &max_length);

			std::vector<GLchar> error_log(max_length);
			glGetShaderInfoLog(shader_id, max_length, &max_length, &error_log[0]);

			glDeleteShader(shader_id);
		}
	}

	size_t shader_id;
	size_t shader_type;
};
