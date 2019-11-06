#pragma once
#include <vector>
#include <string>

struct Mesh
{
public:
	std::vector<float> vertices;
	std::vector<int> indices;
	std::vector<std::tuple<std::string, size_t>> layout;
};
