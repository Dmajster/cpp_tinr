#pragma once
#include <vector>

class Mesh
{
public:
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> uvs;
	std::vector<unsigned int> indices;
};
