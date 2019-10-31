#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "SpriteStrip.h"
#include "asset_loading/ImageLoader.h"
#include "Mesh.h"
#include "Renderer.h"

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

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

	Mesh quad1{
		{
			-1.0f, -0.5f,
			+0.0f, -0.5f,
			+0.0f, +0.5f,
			-1.0f, +0.5f,
		},
		{
			0.0f, 0.0f,
			0.1f, 0.0f,
			0.1f, 0.1f,
			0.0f, 0.1f
		},
		{
			0, 1, 2,
			2, 3, 0,
		}
	};

	Mesh quad2{
		{
			+0.0f, -0.5f,
			+1.0f, -0.5f,
			+1.0f, +0.5f,
			+0.0f, +0.5f
		},
		{
			0.0f, 0.0f,
			0.1f, 0.0f,
			0.1f, 0.1f,
			0.0f, 0.1f
		},
		{
			0, 1, 2,
			2, 3, 0,
		}
	};


	Renderer renderer{};
	renderer.AddMesh(&quad1);
	renderer.AddMesh(&quad2);

	GLfloat const Vertices[] = {
		-1.0f, -0.5f, 0.5f, 0.0f,
		+0.0f, -0.5f, 0.625f, 0.0f,
		+0.0f, +0.5f, 0.625f, 0.3f,
		-1.0f, +0.5f, 0.5f, 0.3f,

		+0.0f, -0.5f, 0.0f, 0.0f,
		+1.0f, -0.5f, 0.125f, 0.0f,
		+1.0f, +0.5f, 0.125f, 0.3f,
		+0.0f, +0.5f, 0.0f, 0.3f
	};

	GLuint const Elements[] = {
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Elements), Elements, GL_STATIC_DRAW);

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

	auto test_sprite_frame = test_sprite_strip.return_frame_by_frame_index(5);

	const auto position_attribute = glGetAttribLocation(shader_program, "position");
	glEnableVertexAttribArray(position_attribute);

	const auto uv_attribute = glGetAttribLocation(shader_program, "uv");
	glEnableVertexAttribArray(uv_attribute);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
	glVertexAttribPointer(uv_attribute, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shader_program);
	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);

	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
	return 0;
}
