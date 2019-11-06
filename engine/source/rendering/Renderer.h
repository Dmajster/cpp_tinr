#pragma once
#include "Mesh.h"
#include "Buffer.h"
#include <map>
#include <memory>

struct RenderMeshData
{
	std::unique_ptr<Buffer> vertices_buffer;
	std::unique_ptr<Buffer> indices_buffer;
	std::unique_ptr<std::vector<std::tuple<std::string, size_t>>> layout;
};

class Renderer
{
public:
	void add_mesh(Mesh* t_mesh)
	{
		auto new_mesh_data = std::make_unique<RenderMeshData>(RenderMeshData{
			std::make_unique<Buffer>(Buffer(GL_ARRAY_BUFFER)),
			std::make_unique<Buffer>(Buffer(GL_ELEMENT_ARRAY_BUFFER)),
			std::make_unique<std::vector<std::tuple<std::string, size_t>>>(t_mesh->layout)
		});

		new_mesh_data->vertices_buffer->bind();
		new_mesh_data->vertices_buffer->set_data(t_mesh->vertices.size() * sizeof(float), t_mesh->vertices.data());
		new_mesh_data->indices_buffer->bind();
		new_mesh_data->indices_buffer->set_data(t_mesh->indices.size() * sizeof(int), t_mesh->indices.data());

		m_mesh_data.insert({
			t_mesh,
			std::move(new_mesh_data)
		});
	}

	void render_mesh(Mesh* t_mesh, Program* t_program)
	{
		t_program->bind();

		const auto mesh_data = m_mesh_data.at(t_mesh).get();
		mesh_data->indices_buffer->bind();
		mesh_data->vertices_buffer->bind();

		auto layout_vector = *mesh_data->layout;
		auto layout_offset = 0;
		auto vertex_size = 0;

		//TODO remove, puke :P
		for (auto layout : layout_vector)
		{
			const auto attribute_size = std::get<1>(layout);
			vertex_size += attribute_size * sizeof(float);
		}

		for (auto layout : layout_vector)
		{
			const auto attribute_name = std::get<0>(layout).c_str();
			const auto attribute_size = std::get<1>(layout);

			//TODO implement abstraction in Program.h
			const auto attribute = glGetAttribLocation(t_program->program_id, attribute_name);
			glEnableVertexAttribArray(attribute);
			glVertexAttribPointer(attribute, attribute_size, GL_FLOAT, GL_FALSE, vertex_size, reinterpret_cast<void*>(layout_offset));
			layout_offset += attribute_size * sizeof(float);
		}

		glDrawElements(GL_TRIANGLES, mesh_data->indices_buffer->buffer_size / sizeof(int), GL_UNSIGNED_INT, nullptr);
	}

	std::map<Mesh*, std::unique_ptr<RenderMeshData>> m_mesh_data;
};
