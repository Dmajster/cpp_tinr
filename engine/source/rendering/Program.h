#pragma once
#include <GL/glew.h>

class Program
{
public:
	Program() : program_id(glCreateProgram())  { }

	~Program()
	{
		glDeleteProgram(program_id);
	}

	void bind() const
	{
		glUseProgram(program_id);
	}

	void link() const
	{
		glLinkProgram(program_id);
	}

	void attach(const Shader& t_shader) const
	{
		glAttachShader(program_id, t_shader.shader_id);
	}

	static void unbind()
	{
		glUseProgram(0);
	}

	const size_t program_id;

	static Program*  create_program_from_shaders(const std::vector<Shader>& t_shaders)
	{
		const auto new_program = new Program{};

		for (const auto& shader : t_shaders)
		{
			new_program->attach(shader);
		}

		new_program->link();

		return new_program;
	}
};
