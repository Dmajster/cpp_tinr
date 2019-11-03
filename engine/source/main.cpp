#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "primitives/SpriteStrip.h"
#include "asset_loading/ImageLoader.h"
#include "rendering/Renderer.h"
#include "primitives/SpriteQuad.h"

#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "primitives/Heightmap.h"
#include "rendering/Shader.h"
#include "asset_loading/ShaderLoader.h"

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

	Shader terrain_vertex_shader(GL_VERTEX_SHADER);
	terrain_vertex_shader.compile(ShaderLoader::load_shader_source("resources/shaders/terrain_vertex.glsl"));

	Shader terrain_fragment_shader(GL_FRAGMENT_SHADER);
	terrain_fragment_shader.compile(ShaderLoader::load_shader_source("resources/shaders/terrain_fragment.glsl"));

	const auto shader_program = glCreateProgram();
	glAttachShader(shader_program, terrain_vertex_shader.shader_id);
	glAttachShader(shader_program, terrain_fragment_shader.shader_id);
	glLinkProgram(shader_program);
	glUseProgram(shader_program);

	const auto test_image = ImageLoader::load_image("resources/textures/test_block_1x1x1_diffuse.png");
	const auto test_sprite_strip = SpriteStrip::bind_and_create_sprite_strip(test_image, 256, 256, 24);

	const auto test_sprite_frame = test_sprite_strip->return_frame_by_frame_index(0);
	SpriteQuad test_sprite(test_sprite_frame);

	const Heightmap test_terrain(50, 50);
	Mesh test_terrain_mesh = test_terrain.create_mesh();

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	Renderer renderer(shader_program);
	renderer.add_mesh(&test_terrain_mesh);
	renderer.add_mesh(&test_sprite);


	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);

	//glm::quat model_rot_x = glm::angleAxis(glm::radians(45.0f), glm::vec3(1, 0, 0));
	//glm::quat model_rot_y = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0));
	//glm::mat4 model = glm::mat4(model_rot_y * model_rot_x);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-25, 0, -25));

	glm::mat4 view = glm::lookAt(
		glm::vec3(-50.0f, 50.0f, -50.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));


	auto size = 12.0f;
	auto z_near = 0.01f;
	auto z_far = 1000.0f;
	glm::mat4 proj = glm::ortho(-size, size, -size, size, z_near, z_far);

	glm::mat4 vp = proj * view;

	const size_t m_uniform_location = glGetUniformLocation(shader_program, "m");
	glUniformMatrix4fv(m_uniform_location, 1, GL_FALSE, value_ptr(model));

	const size_t vp_uniform_location = glGetUniformLocation(shader_program, "vp");
	glUniformMatrix4fv(vp_uniform_location, 1, GL_FALSE, value_ptr(vp));

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.draw_mesh(&test_terrain_mesh);
		renderer.draw_mesh(&test_sprite);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shader_program);
	
	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
	return 0;
}
