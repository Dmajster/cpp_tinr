#pragma once
#include <map>

#include "Mesh.h"
#include "Buffer.h"

struct MeshRenderInformation
{
	size_t position_start;
	size_t position_size;

	size_t index_start;
	size_t index_size;

	size_t uv_start;
	size_t uv_size;
	
	size_t normal_start;
	size_t normal_size;
};

class Renderer
{
public:
	Renderer(GLuint program)
	{
		m_indices = new Buffer(GL_ELEMENT_ARRAY_BUFFER);
		m_indices->bind();
		m_indices->set_space(INT32_MAX, GL_STATIC_DRAW);
		m_indices->unbind();

		m_positions = new Buffer(GL_ARRAY_BUFFER);
		m_positions->bind();
		m_positions->set_space(INT32_MAX, GL_STATIC_DRAW);
		m_positions->unbind();
		
		m_uvs = new Buffer(GL_ARRAY_BUFFER);
		m_uvs->bind();
		m_uvs->set_space(INT32_MAX, GL_STATIC_DRAW);
		m_uvs->unbind();

		m_normals = new Buffer(GL_ARRAY_BUFFER);
		m_normals->bind();
		m_normals->set_space(INT32_MAX, GL_STATIC_DRAW);
		m_normals->unbind();
		
		const auto position_attribute = glGetAttribLocation(program, "vert_position");
		glEnableVertexAttribArray(position_attribute);

		m_positions->bind();
		glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		m_positions->unbind();

		const auto uv_attribute = glGetAttribLocation(program, "vert_uv");
		glEnableVertexAttribArray(uv_attribute);

		m_uvs->bind();
		glVertexAttribPointer(uv_attribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		m_uvs->unbind();

		const auto normal_attribute = glGetAttribLocation(program, "vert_normal");
		glEnableVertexAttribArray(normal_attribute);

		m_normals->bind();
		glVertexAttribPointer(normal_attribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		m_normals->unbind();
	}

	~Renderer()
	{
		free(m_indices);
		free(m_positions);
		free(m_uvs);
		free(m_normals);
	}

	void add_mesh(Mesh* t_mesh)
	{
		MeshRenderInformation mesh_data = {
			m_positions->buffer_last,
			t_mesh->positions.size() * sizeof(float),
			m_indices->buffer_last,
			t_mesh->indices.size() * sizeof(int),
			m_uvs->buffer_last,
			t_mesh->uvs.size() * sizeof(float),
			m_normals->buffer_last,
			t_mesh->normals.size() * sizeof(float)
		};

		m_mesh_data.insert({ t_mesh, mesh_data });

		m_positions->bind();
		m_positions->push_data(t_mesh->positions.size() * sizeof(float), t_mesh->positions.data());
		m_positions->unbind();
		
		m_indices->bind();
		m_indices->push_data(t_mesh->indices.size() * sizeof(int), t_mesh->indices.data());
		m_indices->unbind();
		
		m_uvs->bind();
		m_uvs->push_data(t_mesh->uvs.size() * sizeof(float), t_mesh->uvs.data());
		m_uvs->unbind();

		m_normals->bind();
		m_normals->push_data(t_mesh->normals.size() * sizeof(float), t_mesh->normals.data());
		m_normals->unbind();
	}

	void draw_mesh(Mesh* t_mesh)
	{
		m_indices->bind();
		const auto mesh_data = m_mesh_data.at(t_mesh);
		
		glDrawElements(GL_TRIANGLES, mesh_data.index_size / sizeof(int), GL_UNSIGNED_INT, reinterpret_cast<void*>(mesh_data.index_start));
	}

private:
	std::map<Mesh*, MeshRenderInformation> m_mesh_data;

	Buffer* m_indices;
	Buffer* m_positions;
	Buffer* m_normals;
	Buffer* m_uvs;
};
