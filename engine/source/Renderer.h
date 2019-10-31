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
};

class Renderer
{
public:
	Renderer()
	{
		m_indices = new Buffer(GL_ELEMENT_ARRAY_BUFFER);
		m_indices->set_space(1024, GL_STATIC_DRAW);
		
		m_positions = new Buffer(GL_ARRAY_BUFFER);
		m_positions->set_space(1024, GL_STATIC_DRAW);

		m_uvs = new Buffer(GL_ARRAY_BUFFER);
		m_uvs->set_space(1024, GL_STATIC_DRAW);
	}

	~Renderer()
	{
		free(m_indices);
		free(m_positions);
		free(m_uvs);
	}

	void AddMesh(Mesh* t_mesh)
	{
		MeshRenderInformation mesh_data = {
			m_positions->buffer_last,
			t_mesh->positions.size() * sizeof(float),
			m_indices->buffer_last,
			t_mesh->indices.size() * sizeof(int),
			m_uvs->buffer_last,
			t_mesh->uvs.size() * sizeof(float)
		};

		m_mesh_data.insert({ t_mesh, mesh_data });
		
		m_indices->push_data(t_mesh->indices.size() * sizeof(int), &t_mesh->indices.at(0));
		m_positions->push_data(t_mesh->positions.size() * sizeof(float), &t_mesh->positions.at(0));
		m_uvs->push_data(t_mesh->uvs.size() * sizeof(float), &t_mesh->uvs.at(0));
	}
	
private:
	std::map<Mesh*, MeshRenderInformation> m_mesh_data;
	
	Buffer* m_indices;
	Buffer* m_positions;
	
	Buffer* m_uvs;
};
