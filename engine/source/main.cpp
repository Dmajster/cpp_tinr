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
#include "rendering/Program.h"

int main()
{
	Window window{};
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);

	auto terrain_program = Program::create_program_from_shaders({
		Shader(ShaderType::vertex, ShaderLoader::load_shader_source("resources/shaders/terrain.vert")),
		Shader(ShaderType::fragment, ShaderLoader::load_shader_source("resources/shaders/terrain.frag")),
		});

	auto sprite_program = Program::create_program_from_shaders({
		Shader(ShaderType::vertex, ShaderLoader::load_shader_source("resources/shaders/sprite.vert")),
		Shader(ShaderType::fragment, ShaderLoader::load_shader_source("resources/shaders/sprite.frag")),
		});

	auto line_program = Program::create_program_from_shaders({
		Shader(ShaderType::vertex,	ShaderLoader::load_shader_source("resources/shaders/line.vert")),
		Shader(ShaderType::fragment,	ShaderLoader::load_shader_source("resources/shaders/line.frag")),
		});

	auto test_image = ImageLoader::load_image("resources/textures/test_block_1x1x1_diffuse.png");
	const auto test_sprite_strip = SpriteStrip::bind_and_create_sprite_strip(&test_image, 256, 256, 24);

	const auto test_sprite_frame = test_sprite_strip->return_frame_by_frame_index(0);
	SpriteQuad test_sprite(*test_sprite_frame);

	const auto test_sprite_frame_2 = test_sprite_strip->return_frame_by_frame_index(16);
	SpriteQuad test_sprite_2(*test_sprite_frame_2);

	const Heightmap test_terrain(5, 5);
	auto test_terrain_mesh = test_terrain.create_mesh();

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	Renderer renderer{};
	renderer.add_mesh(&test_sprite);
	renderer.add_mesh(&test_sprite_2);
	renderer.add_mesh(&test_terrain_mesh);
	
	//glm::mat4 model = glm::mat4(model_rot_y * model_rot_x);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, 0, -2.5f));

	glm::mat4 view = glm::lookAt(
		glm::vec3(-50.0f, 50.0f, -50.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));


	auto size = 5.0f;
	auto z_near = 0.01f;
	auto z_far = 1000.0f;
	glm::mat4 proj = glm::ortho(-size, size, -size, size, z_near, z_far);

	glm::mat4 vp = proj * view;




	terrain_program->bind();
	renderer.bind_program(terrain_program->program_id);

	size_t m_uniform_location = glGetUniformLocation(terrain_program->program_id, "m");
	glUniformMatrix4fv(m_uniform_location, 1, GL_FALSE, value_ptr(model));

	size_t vp_uniform_location = glGetUniformLocation(terrain_program->program_id, "vp");
	glUniformMatrix4fv(vp_uniform_location, 1, GL_FALSE, value_ptr(vp));

	line_program->bind();
	renderer.bind_program(line_program->program_id);

	m_uniform_location = glGetUniformLocation(line_program->program_id, "m");
	glUniformMatrix4fv(m_uniform_location, 1, GL_FALSE, value_ptr(model));

	vp_uniform_location = glGetUniformLocation(line_program->program_id, "vp");
	glUniformMatrix4fv(vp_uniform_location, 1, GL_FALSE, value_ptr(vp));

	sprite_program->bind();
	renderer.bind_program(sprite_program->program_id);

	glm::quat model_rot_x = glm::angleAxis(glm::radians(45.0f), glm::vec3(1, 0, 0));
	glm::quat model_rot_y = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0));
	model = glm::mat4(model_rot_y * model_rot_x);

	auto model_1 = glm::translate(model, glm::vec3(1, 1, 3));
	auto model_2 = glm::translate(model, glm::vec3(-1, 1, 1));

	m_uniform_location = glGetUniformLocation(sprite_program->program_id, "m");
	glUniformMatrix4fv(m_uniform_location, 1, GL_FALSE, value_ptr(model));



	vp_uniform_location = glGetUniformLocation(sprite_program->program_id, "vp");
	glUniformMatrix4fv(vp_uniform_location, 1, GL_FALSE, value_ptr(vp));

	glEnable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.0f);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	while (!window.should_close())
	{
		Window::clear(ClearType::color);
		Window::clear(ClearType::depth);

		terrain_program->bind();
		glUniformMatrix4fv(m_uniform_location, 1, GL_FALSE, value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, 0, -2.5f))));
		renderer.bind_program(terrain_program->program_id);
		glDrawElementsBaseVertex(GL_TRIANGLES, 128 , GL_UNSIGNED_INT, nullptr, 8);
		
		//renderer.draw_mesh(&test_terrain_mesh);
			
		line_program->bind();
		glUniformMatrix4fv(m_uniform_location, 1, GL_FALSE, value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, 0.02, -2.5f))));
		//renderer.draw_mesh(&test_terrain_mesh, GL_LINES);
		glDrawElementsBaseVertex(GL_LINES, 128, GL_UNSIGNED_INT, nullptr, 8);
		
		sprite_program->bind();
		renderer.bind_program(sprite_program->program_id);

		glUniformMatrix4fv(m_uniform_location, 1, GL_FALSE, value_ptr(model_1));
		glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, 0);
		//renderer.draw_mesh(&test_sprite);
		
		glUniformMatrix4fv(m_uniform_location, 1, GL_FALSE, value_ptr(model_2));
		glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, 4);
		//renderer.draw_mesh(&test_sprite_2);



		window.swap_buffers();
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
	return 0;
}
