#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"

#include "primitives/SpriteStrip.h"
#include "primitives/SpriteQuad.h"

#include "asset_loading/ShaderLoader.h"
#include "asset_loading/ImageLoader.h"

#include "rendering/Window.h"
#include "rendering/Shader.h"
#include "rendering/Program.h"
#include "rendering/Renderer.h"
#include "primitives/Map.h"

#include "entt/entt.hpp"

int main()
{
	Window window{};
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	auto sprite_program = Program::create_program_from_shaders({
		Shader(ShaderType::vertex, ShaderLoader::load_shader_source("resources/shaders/sprite.vert")),
		Shader(ShaderType::fragment, ShaderLoader::load_shader_source("resources/shaders/sprite.frag")),
		});

	auto terrain_program = Program::create_program_from_shaders({
		Shader(ShaderType::vertex, ShaderLoader::load_shader_source("resources/shaders/terrain.vert")),
		Shader(ShaderType::fragment, ShaderLoader::load_shader_source("resources/shaders/terrain.frag")),
		});

	auto line_program = Program::create_program_from_shaders({
		Shader(ShaderType::vertex, ShaderLoader::load_shader_source("resources/shaders/line.vert")),
		Shader(ShaderType::fragment, ShaderLoader::load_shader_source("resources/shaders/line.frag")),
		});

	
	sprite_program->bind();

	auto test_image = ImageLoader::load_image("resources/textures/test_block_1x1x1_diffuse.png");
	const auto test_sprite_strip = SpriteStrip::bind_and_create_sprite_strip(&test_image, 256, 256, 24);

	const auto test_sprite_frame_1 = test_sprite_strip->return_frame_by_frame_index(0);
	SpriteQuad test_sprite_1(*test_sprite_frame_1);

	const auto test_sprite_frame_2 = test_sprite_strip->return_frame_by_frame_index(16);
	SpriteQuad test_sprite_2(*test_sprite_frame_2);

	auto test_map = Map(10, 10);

	Renderer renderer;
	renderer.add_mesh(&test_map);
	renderer.add_mesh(&test_sprite_1);
	renderer.add_mesh(&test_sprite_2);

	glm::quat model_rot_x = glm::angleAxis(glm::radians(45.0f), glm::vec3(1, 0, 0));
	glm::quat model_rot_y = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0));
	glm::mat4 model = glm::mat4(model_rot_y * model_rot_x);

	auto m_terrain = glm::mat4(1.0);
	auto m1_t = glm::translate(glm::mat4(1.0), glm::vec3(1, 0, 0));
	auto m2_t = glm::translate(glm::mat4(1.0), glm::vec3(-1, 0, 0));
	auto m1_r = model;
	auto m1_s = glm::mat4(1.0);

	auto m1 = m1_t * m1_r * m1_s;
	auto m2 = m2_t * m1_r * m1_s;

	const auto view = glm::lookAt(
		glm::vec3(-50.0f, 50.0f, -50.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	const auto size = 5.0f;
	const auto z_near = 0.01f;
	const auto z_far = 1000.0f;
	const auto proj = glm::ortho(-size, size, -size, size, z_near, z_far);

	auto vp = proj * view;

	sprite_program->bind();
	const size_t m_uniform_location = glGetUniformLocation(sprite_program->program_id, "m");
	const size_t vp_uniform_location = glGetUniformLocation(sprite_program->program_id, "vp");
	glUniformMatrix4fv(vp_uniform_location, 1, GL_FALSE, value_ptr(vp));

	terrain_program->bind();
	const size_t m_ter_uniform_location = glGetUniformLocation(terrain_program->program_id, "m");
	const size_t vp_ter_uniform_location = glGetUniformLocation(terrain_program->program_id, "vp");
	glUniformMatrix4fv(vp_ter_uniform_location, 1, GL_FALSE, value_ptr(vp));

	line_program->bind();
	const size_t m_lin_uniform_location = glGetUniformLocation(line_program->program_id, "m");
	const size_t vp_lin_uniform_location = glGetUniformLocation(line_program->program_id, "vp");
	glUniformMatrix4fv(vp_lin_uniform_location, 1, GL_FALSE, value_ptr(vp));

	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLineWidth(1.0f);
	
	while (!window.should_close())
	{
		//TODO deal with the bit masking somehow. this is horrible.
		Window::clear(ClearType::color);
		Window::clear(ClearType::depth);

		terrain_program->bind();
		glUniformMatrix4fv(m_ter_uniform_location, 1, GL_FALSE, value_ptr(m_terrain));
		renderer.render_mesh(&test_map, terrain_program);

		line_program->bind();
		glUniformMatrix4fv(m_lin_uniform_location, 1, GL_FALSE, value_ptr(glm::translate(m_terrain, glm::vec3(0, 0.02, 0))));
		renderer.render_mesh(&test_map, line_program, GL_LINES);

		sprite_program->bind();
		glUniformMatrix4fv(m_uniform_location, 1, GL_FALSE, value_ptr(m1));
		renderer.render_mesh(&test_sprite_1, sprite_program);

		glUniformMatrix4fv(m_uniform_location, 1, GL_FALSE, value_ptr(m2));
		renderer.render_mesh(&test_sprite_2, sprite_program);

		window.swap_buffers();
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
	return 0;
}
