#pragma once
#include "Mesh.h"

class Heightmap
{
public:
	size_t width;
	size_t length;
	float* heights;

	Heightmap(const size_t t_width, const size_t t_height) : width(t_width), length(t_height)
	{
		heights = static_cast<float*>(calloc(t_width * t_height, sizeof(float)));
		const auto element_count = t_width * t_height;

		for (auto i = 0; i < element_count; i++)
		{
			heights[i] = static_cast<float>(rand()) / RAND_MAX;
		}
	}

	Mesh create_mesh() const
	{
		Mesh return_mesh;

		for (auto z = 0; z < length; z++)
		{
			for (auto x = 0; x < width; x++)
			{
				return_mesh.positions.push_back(x);
				return_mesh.positions.push_back(heights[vector_to_index(x, z)]);
				return_mesh.positions.push_back(z);

				if (x + 1 < width && z + 1 < length)
				{
					return_mesh.indices.push_back(vector_to_index(x, z));
					return_mesh.indices.push_back(vector_to_index(x + 1, z));
					return_mesh.indices.push_back(vector_to_index(x + 1, z + 1));

					return_mesh.indices.push_back(vector_to_index(x + 1, z + 1));
					return_mesh.indices.push_back(vector_to_index(x, z + 1));
					return_mesh.indices.push_back(vector_to_index(x, z));
				}

				auto p0 = glm::vec3(x, heights[vector_to_index(x, z)], z);
				auto p1 = glm::vec3(x + 1, heights[vector_to_index(x + 1, z)], z);
				auto p2 = glm::vec3(x, heights[vector_to_index(x, z + 1)], z + 1);
				auto normal = glm::normalize(glm::cross(p2 - p0, p1 - p0));

				return_mesh.normals.push_back(normal.x);
				return_mesh.normals.push_back(normal.y);
				return_mesh.normals.push_back(normal.z);
			}
		}

		return return_mesh;
	}

	size_t vector_to_index(const size_t x, const size_t y) const
	{
		return y * width + x;
	}
};
