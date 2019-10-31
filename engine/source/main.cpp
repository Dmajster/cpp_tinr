#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "SpriteStrip.h"
#include "asset_loading/ImageLoader.h"
#include "Mesh.h"
#include "Renderer.h"
#include "SpriteQuad.h"

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	glfwSetErrorCallback(error_callback);

	window = glfwCreateWindow(640, 640, "Hello World", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	const GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		glfwTerminate();
		return -1;
	}

	char const* VertexShaderSource = R"GLSL(
		#version 150
		in vec2 position;
		in vec2 uv;

		out vec2 frag_uv;
		void main()
		{
			frag_uv = uv;
			gl_Position = vec4(position, 0.0, 1.0);
		}
	)GLSL";

	char const* FragmentShaderSource = R"GLSL(
		#version 150
		in vec2 frag_uv;

		uniform sampler2D sprite;
	
		out vec4 outColor;
		void main()
		{
			outColor = texture(sprite, frag_uv);
		}
	)GLSL";

	GLint compiled;
	const auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &VertexShaderSource, nullptr);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		std::cerr << "Failed to compile vertex shader!" << std::endl;
	}

	const auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &FragmentShaderSource, nullptr);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		std::cerr << "Failed to compile fragment shader!" << std::endl;
	}

	const auto shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glBindFragDataLocation(shader_program, 0, "outColor");
	glLinkProgram(shader_program);
	glUseProgram(shader_program);

	const auto test_image = ImageLoader::loadImage("resources/test_block_1x1x1_diffuse.png");
	const auto test_sprite_strip = SpriteStrip::bind_and_create_sprite_strip(test_image, 256, 256, 24);

	const auto test_sprite_frame = test_sprite_strip->return_frame_by_frame_index(4);
	SpriteQuad test_sprite(test_sprite_frame);


	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	Renderer renderer(shader_program);
	renderer.AddMesh(&test_sprite);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.DrawMesh(&test_sprite);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shader_program);
	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);

	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
	return 0;
}
