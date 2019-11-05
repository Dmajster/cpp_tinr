#pragma once
#include "../rendering/Mesh.h"

class Heightmap
{
public:
	size_t width;
	size_t length;
	float* heights;

	Heightmap(const size_t t_width, const size_t t_height) : width(t_width), length(t_height)
	{
		heights = static_cast<float*>(calloc(t_width * t_height, sizeof(float)));

		//const auto element_count = t_width * t_height;

		//for (auto i = 0; i < element_count; i++)
		//{
		//	heights[i] = static_cast<float>(rand()) / RAND_MAX;
		//}
	}

	Mesh create_mesh() const
	{
		Mesh return_mesh;

		for (auto z = 0; z < length; z++)
		{
			for (auto x = 0; x < width; x++)
			{
				if (x + 1 >= width || z + 1 >= length)
				{
					continue;
				}

				return_mesh.positions.push_back(x);
				return_mesh.positions.push_back(heights[vector_to_index(x, z)]);
				return_mesh.positions.push_back(z);

				return_mesh.positions.push_back(x + 1);
				return_mesh.positions.push_back(heights[vector_to_index(x + 1, z)]);
				return_mesh.positions.push_back(z);

				return_mesh.positions.push_back(x + 1);
				return_mesh.positions.push_back(heights[vector_to_index(x + 1, z + 1)]);
				return_mesh.positions.push_back(z + 1);

				return_mesh.positions.push_back(x);
				return_mesh.positions.push_back(heights[vector_to_index(x, z + 1)]);
				return_mesh.positions.push_back(z + 1);

				const auto size = return_mesh.positions.size() / 3;

				return_mesh.indices.push_back(size - 4);
				return_mesh.indices.push_back(size - 3);
				return_mesh.indices.push_back(size - 2);

				return_mesh.indices.push_back(size - 2);
				return_mesh.indices.push_back(size - 1);
				return_mesh.indices.push_back(size - 4);


				const auto p0 = glm::vec3(x, heights[vector_to_index(x, z)], z);
				const auto p1 = glm::vec3(x + 1, heights[vector_to_index(x + 1, z)], z);
				const auto p2 = glm::vec3(x + 1, heights[vector_to_index(x + 1, z + 1)], z + 1);
				const auto p3 = glm::vec3(x, heights[vector_to_index(x, z + 1)], z + 1);

				auto normal_tris_1 = glm::normalize(glm::cross(p2 - p0, p1 - p0));
				auto normal_tris_2 = glm::normalize(glm::cross(p2 - p3, p1 - p3));
				auto normal_average = glm::normalize((normal_tris_1 + normal_tris_2) / 2.0f);
				
				return_mesh.normals.push_back(normal_tris_2.x);
				return_mesh.normals.push_back(normal_tris_2.y);
				return_mesh.normals.push_back(normal_tris_2.z);

				return_mesh.normals.push_back(normal_tris_2.x);
				return_mesh.normals.push_back(normal_tris_2.y);
				return_mesh.normals.push_back(normal_tris_2.z);

				return_mesh.normals.push_back(normal_tris_2.x);
				return_mesh.normals.push_back(normal_tris_2.y);
				return_mesh.normals.push_back(normal_tris_2.z);

				return_mesh.normals.push_back(normal_tris_2.x);
				return_mesh.normals.push_back(normal_tris_2.y);
				return_mesh.normals.push_back(normal_tris_2.z);
			}
		}

		return return_mesh;
	}

	size_t vector_to_index(const size_t x, const size_t y) const
	{
		return y * width + x;
	}
};
