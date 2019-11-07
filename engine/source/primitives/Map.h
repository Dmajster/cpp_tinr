#pragma once
#include "../rendering/Mesh.h"
#include "glm/ext.hpp"

#include <vector>

class Map : public Mesh
{
public:
	const size_t m_width;
	const size_t m_length;
	std::vector<float> m_heights;

	Map(const size_t t_width, const size_t t_length) : m_width(t_width), m_length(t_length), m_heights(std::vector<float>(m_width* m_length))
	{
		m_heights.at(vector_to_index(3,3)) = 1.5f;
		m_heights.at(vector_to_index(4, 3)) = 1.5f;
		m_heights.at(vector_to_index(4, 4)) = 1.5f;
		m_heights.at(vector_to_index(3, 4)) = 1.5f;
		
		for (auto z = 0; z < m_length; z++)
		{
			for (auto x = 0; x < m_width; x++)
			{
				if (x + 1 >= m_width || z + 1 >= m_length)
				{
					continue;
				}

				const auto p0 = glm::vec3(x, m_heights[vector_to_index(x, z)], z);
				const auto p1 = glm::vec3(x + 1, m_heights[vector_to_index(x + 1, z)], z);
				const auto p2 = glm::vec3(x + 1, m_heights[vector_to_index(x + 1, z + 1)], z + 1);
				const auto p3 = glm::vec3(x, m_heights[vector_to_index(x, z + 1)], z + 1);

				auto normal_tris_1 = glm::normalize(glm::cross(p2 - p0, p1 - p0));
				auto normal_tris_2 = glm::normalize(glm::cross(p2 - p3, p1 - p3));
				auto normal_average = glm::normalize((normal_tris_1 + normal_tris_2) / 2.0f);

				vertices.push_back(x);
				vertices.push_back(m_heights[vector_to_index(x, z)]);
				vertices.push_back(z);

				vertices.push_back(normal_average.x);
				vertices.push_back(normal_average.y);
				vertices.push_back(normal_average.z);
				
				vertices.push_back(x + 1);
				vertices.push_back(m_heights[vector_to_index(x + 1, z)]);
				vertices.push_back(z);

				vertices.push_back(normal_average.x);
				vertices.push_back(normal_average.y);
				vertices.push_back(normal_average.z);
				
				vertices.push_back(x + 1);
				vertices.push_back(m_heights[vector_to_index(x + 1, z + 1)]);
				vertices.push_back(z + 1);

				vertices.push_back(normal_average.x);
				vertices.push_back(normal_average.y);
				vertices.push_back(normal_average.z);
				
				vertices.push_back(x);
				vertices.push_back(m_heights[vector_to_index(x, z + 1)]);
				vertices.push_back(z + 1);
				
				vertices.push_back(normal_average.x);
				vertices.push_back(normal_average.y);
				vertices.push_back(normal_average.z);

				const auto size = vertices.size() / 6;

				indices.push_back(size - 4);
				indices.push_back(size - 3);
				indices.push_back(size - 2);

				indices.push_back(size - 2);
				indices.push_back(size - 1);
				indices.push_back(size - 4);
			}
		}

		layout = {
			{ "in_position", 3 },
			{ "in_normal", 3 }
		};
	}

	size_t vector_to_index(const size_t x, const size_t y) const
	{
		return y * m_width + x;
	}
};
