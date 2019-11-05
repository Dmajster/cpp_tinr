#pragma once
#include <GL/glew.h>
#include <vector>

enum class ShaderType
{
	vertex = GL_VERTEX_SHADER,
	fragment = GL_FRAGMENT_SHADER
};

class Shader
{
public:
	explicit Shader(const ShaderType t_shader_type, const std::string& t_shader_source) :
		shader_id(glCreateShader(static_cast<GLenum>(t_shader_type))),
		shader_type(t_shader_type)
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

			const std::string error_string(error_log.begin(), error_log.end());

			std::cout << error_string << std::endl;
		}
	}

	~Shader()
	{
		glDeleteShader(shader_id);
	}

	size_t shader_id;
	ShaderType shader_type;
};
