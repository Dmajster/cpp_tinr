#pragma once
#include <GL/glew.h>

class Program
{
public:
	Program() : program_id(glCreateProgram()) { }

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

	static std::unique_ptr<Program> create_program_from_shaders(const std::vector<Shader>& t_shaders)
	{
		auto new_program = std::make_unique<Program>();

		for (const auto& shader : t_shaders)
		{
			new_program->attach(shader);
		}

		new_program->link();

		return new_program;
	}

	int32_t get_uniform_location(const std::string& t_name) const
	{
		return glGetUniformLocation(program_id, t_name.c_str());
	}

	void set_uniform(const std::string& t_name, const glm::mat4& value) const
	{
		const auto uniform_location = get_uniform_location(t_name);

		glUniformMatrix4fv(uniform_location, 1, GL_FALSE, value_ptr(value));
	}
};
