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

#include "ecs/System.h"
#include "components/RenderComponent.h"
#include "components/PositionComponent.h"
#include "components/OrthoCameraComponent.h"
#include "input/Input.h"
#include "rendering/Material.h"

void render_sprites(Program program, entt::registry& registry)
{
	auto& renderer = registry.ctx<Renderer>();

	auto view = registry.view<RenderComponent, PositionComponent>();
	for (auto entity : view)
	{
		auto& mesh = view.get<RenderComponent>(entity);
		auto& position = view.get<PositionComponent>(entity);
		
		program.bind();
		program.set_uniform("m", glm::translate(glm::mat4(1.0f), position.position_in_meters));
		renderer.render_mesh(mesh.mesh, program);
	}
}

void mesh_added(entt::entity entity, entt::registry& registry, RenderComponent& render_component)
{
	printf("Mesh added!\n");

	auto &renderer = registry.ctx<Renderer>();
	renderer.add_mesh(render_component.mesh);
}


void key_callback(GLFWwindow* window, const int key, const int scancode, const int action, int mods)
{
	printf("pressed key: %d, action: %d\n", key, action);
}

static void cursor_position_callback(GLFWwindow* window, const double xpos, const double ypos)
{
	printf("mouse position: (%f,%f)\n", xpos, ypos);
}

int main()
{
	const auto window = std::make_unique<Window>();
	const auto input = std::make_unique<Input>(*window);
	const auto renderer = std::make_unique<Renderer>();

	entt::registry game_registry;
	game_registry.set<Renderer>(*renderer);
	game_registry.on_construct<RenderComponent>().connect<&mesh_added>();
	
	glfwSetKeyCallback(window->window, key_callback);
	glfwSetCursorPosCallback(window->window, cursor_position_callback);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	auto sprite_program = Program::create_program_from_shaders({
		Shader(ShaderType::vertex, ShaderLoader::load_shader_source("resources/shaders/sprite.vert")),
		Shader(ShaderType::fragment, ShaderLoader::load_shader_source("resources/shaders/sprite.frag")),
	});

	auto sprite_material = Material {
		sprite_program,
	};
	
	sprite_program->bind();

	auto test_image = ImageLoader::load_image("resources/textures/test_block_1x1x1_diffuse.png");
	const auto test_sprite_strip = SpriteStrip::bind_and_create_sprite_strip(&test_image, 256, 256, 24);

	const auto test_sprite_frame_1 = test_sprite_strip->return_frame_by_frame_index(0);
	SpriteQuad test_sprite_1(*test_sprite_frame_1);

	const auto test_sprite_frame_2 = test_sprite_strip->return_frame_by_frame_index(16);
	SpriteQuad test_sprite_2(*test_sprite_frame_2);

	const auto entity_1 = game_registry.create();
	game_registry.assign<PositionComponent>(entity_1, PositionComponent{ glm::vec3(1, 0, 0) });
	game_registry.assign<RenderComponent>(entity_1, RenderComponent{ &test_sprite_1, &sprite_material });

	const auto entity_2 = game_registry.create();
	game_registry.assign<PositionComponent>(entity_2, PositionComponent{ glm::vec3(-1, 0, 0) });
	game_registry.assign<RenderComponent>(entity_2, RenderComponent{ &test_sprite_2, &sprite_material });

	const auto camera = game_registry.create();
	game_registry.assign<PositionComponent>(camera, PositionComponent{ glm::vec3(0, 0, 0) });
	game_registry.assign<OrthoCameraComponent>(camera, OrthoCameraComponent{ 5.0f, 0.01, 1000.0f });

	const auto view = glm::lookAt(
		glm::vec3(-50.0f, 50.0f, -50.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	const auto size = 5.0f;
	const auto z_near = 0.01f;
	const auto z_far = 1000.0f;
	const auto proj = glm::ortho(-size, size, -size, size, z_near, z_far);

	const auto vp = proj * view;

	sprite_program->bind();
	sprite_program->set_uniform("m", glm::mat4(1.0f));
	sprite_program->set_uniform("vp", vp);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLineWidth(1.0f);

	while (!window->should_close())
	{
		//TODO deal with the bit masking somehow. this is horrible.
		Window::clear(ClearType::color);
		Window::clear(ClearType::depth);

		render_sprites(*sprite_program, game_registry);


		window->swap_buffers();
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
	return 0;
}
