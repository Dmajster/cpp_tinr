#pragma once
#include <fstream>

class ShaderLoader
{
public:
	static std::string load_shader_source(const char* t_path)
	{
		std::ifstream t(t_path);
		std::string shader_string;

		t.seekg(0, std::ios::end);
		shader_string.reserve(t.tellg());
		t.seekg(0, std::ios::beg);

		shader_string.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

		return shader_string;
	}
};
