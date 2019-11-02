#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "SpriteStrip.h"
#include "asset_loading/ImageLoader.h"
#include "Renderer.h"
#include "SpriteQuad.h"

#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "Heightmap.h"

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
		in vec3 position;
		in vec3 normal;
		in vec2 uv;

		uniform mat4 mvp;
	
		out vec2 frag_uv;
		out vec3 frag_normal;
		void main()
		{
			frag_uv = uv;
			frag_normal = normal;
			gl_Position = mvp * vec4(position, 1.0);
		}
	)GLSL";

	char const* FragmentShaderSource = R"GLSL(
		#version 150
		in vec2 frag_uv;
		in vec3 frag_normal;

		uniform sampler2D sprite;
	
		out vec4 outColor;
		void main()
		{
			//outColor = vec4(frag_normal, 1.0f);
			outColor = vec4(frag_normal, 1.0f) * vec4(1.0f,1.0f,1.0f,1.0f);
			//outColor = texture(sprite, frag_uv);
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

	const auto test_sprite_frame = test_sprite_strip->return_frame_by_frame_index(0);
	SpriteQuad test_sprite(test_sprite_frame);

	const Heightmap test_terrain(50, 50);
	Mesh test_terrain_mesh = test_terrain.create_mesh();
	
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	Renderer renderer(shader_program);
	renderer.AddMesh(&test_terrain_mesh);
	//renderer.AddMesh(&test_sprite);

	
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);

	//glm::quat model_rot_x = glm::angleAxis(glm::radians(45.0f), glm::vec3(1, 0, 0));
	//glm::quat model_rot_y = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0));
	//glm::mat4 model = glm::mat4(model_rot_y * model_rot_x);

	glm::mat4 model = glm::mat4(1.0f);
	
	glm::mat4 view = glm::lookAt(
		glm::vec3(-5.0f, 5.0f, -5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));


	auto size = 10.0f;
	auto z_near = 0.01f;
	auto z_far = 100.0f;
	glm::mat4 proj = glm::ortho(-size, size, -size, size, z_near, z_far);

	glm::mat4 mvp = proj * view * model;

	const size_t mvp_uniform_location = glGetUniformLocation(shader_program, "mvp");
	glUniformMatrix4fv(mvp_uniform_location, 1, GL_FALSE, value_ptr(mvp));

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.DrawMesh(&test_terrain_mesh);
		//renderer.DrawMesh(&test_sprite);

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
