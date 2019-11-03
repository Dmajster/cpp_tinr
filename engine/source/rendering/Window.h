#pragma once
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <iostream>

enum class ClearType
{
	color = GL_COLOR_BUFFER_BIT,
	depth = GL_DEPTH_BUFFER_BIT,
	accumulation = GL_ACCUM_BUFFER_BIT,
	stencil = GL_STENCIL_BUFFER_BIT
};

class Window
{
public:
	static void error_callback(int error, const char* description)
	{
		fprintf(stderr, "Error: %s\n", description);
		throw;
	}

	explicit Window(const size_t t_window_width = 640, const size_t t_window_height = 480, const std::string& t_window_title = "Title") :
		window_height(t_window_height),
		window_width(t_window_width)
	{
		if (!glfwInit())
			throw;

		glfwSetErrorCallback(error_callback);

		window = glfwCreateWindow(t_window_width, t_window_height, t_window_title.c_str(), nullptr, nullptr);

		if (!window)
		{
			glfwTerminate();
			throw;
		}
		
		bind();
		
		const auto err = glewInit();

		if (GLEW_OK != err)
		{
			std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
			glfwTerminate();
			throw;
		}
	}

	void bind() const
	{
		glfwMakeContextCurrent(window);
	}
	
	bool should_close() const
	{
		return glfwWindowShouldClose(window);
	}

	static void clear(const ClearType t_clear_mask)
	{
		glClear(static_cast<GLbitfield>(t_clear_mask));
	}

	void swap_buffers() const
	{
		glfwSwapBuffers(window);
	}

	GLFWwindow* window;
	size_t window_height;
	size_t window_width;
};
