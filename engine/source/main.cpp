#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"

#include "primitives/SpriteStrip.h"
#include "primitives/Heightmap.h"
#include "primitives/SpriteQuad.h"

#include "asset_loading/ShaderLoader.h"
#include "asset_loading/ImageLoader.h"

#include "rendering/Window.h"
#include "rendering/Shader.h"
#include "rendering/Renderer.h"

int main()
{
	Window window{};
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
	
	
	Shader terrain_vertex_shader(ShaderType::vertex);
	terrain_vertex_shader.compile(ShaderLoader::load_shader_source("resources/shaders/terrain.vert"));

	Shader terrain_fragment_shader(ShaderType::fragment);
	terrain_fragment_shader.compile(ShaderLoader::load_shader_source("resources/shaders/terrain.frag"));

	Shader sprite_vertex_shader(ShaderType::vertex);
	sprite_vertex_shader.compile(ShaderLoader::load_shader_source("resources/shaders/sprite.vert"));

	Shader sprite_fragment_shader(ShaderType::fragment);
	sprite_fragment_shader.compile(ShaderLoader::load_shader_source("resources/shaders/sprite.frag"));

	const auto terrain_program = glCreateProgram();
	glAttachShader(terrain_program, terrain_vertex_shader.shader_id);
	glAttachShader(terrain_program, terrain_fragment_shader.shader_id);
	glLinkProgram(terrain_program);
	glUseProgram(terrain_program);

	const auto sprite_program = glCreateProgram();
	glAttachShader(sprite_program, sprite_vertex_shader.shader_id);
	glAttachShader(sprite_program, sprite_fragment_shader.shader_id);
	glLinkProgram(sprite_program);
	glUseProgram(sprite_program);
	
	const auto test_image = ImageLoader::load_image("resources/textures/test_block_1x1x1_diffuse.png");
	const auto test_sprite_strip = SpriteStrip::bind_and_create_sprite_strip(test_image, 256, 256, 24);

	const auto test_sprite_frame = test_sprite_strip->return_frame_by_frame_index(0);
	SpriteQuad test_sprite(test_sprite_frame);

	const Heightmap test_terrain(50, 50);
	auto test_terrain_mesh = test_terrain.create_mesh();

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	Renderer renderer{};
	renderer.add_mesh(&test_terrain_mesh);
	renderer.add_mesh(&test_sprite);

	//glUseProgram(terrain_program);
	//renderer.bind_program(terrain_program);

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

	glUseProgram(sprite_program);
	renderer.bind_program(sprite_program);
	
	size_t m_uniform_location = glGetUniformLocation(terrain_program, "m");
	glUniformMatrix4fv(m_uniform_location, 1, GL_FALSE, value_ptr(model));

	size_t vp_uniform_location = glGetUniformLocation(terrain_program, "vp");
	glUniformMatrix4fv(vp_uniform_location, 1, GL_FALSE, value_ptr(vp));

	glUseProgram(terrain_program);
	renderer.bind_program(terrain_program);

	m_uniform_location = glGetUniformLocation(terrain_program, "m");
	glUniformMatrix4fv(m_uniform_location, 1, GL_FALSE, value_ptr(model));

	vp_uniform_location = glGetUniformLocation(terrain_program, "vp");
	glUniformMatrix4fv(vp_uniform_location, 1, GL_FALSE, value_ptr(vp));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	
	while (!window.should_close())
	{
		Window::clear(ClearType::color);
		Window::clear(ClearType::depth);

		glUseProgram(terrain_program);
		renderer.draw_mesh(&test_terrain_mesh);

		glUseProgram(sprite_program);
		renderer.draw_mesh(&test_terrain_mesh, GL_LINES);

		window.swap_buffers();
		glfwPollEvents();
	}

	glDeleteProgram(terrain_program);
	
	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
	return 0;
}
