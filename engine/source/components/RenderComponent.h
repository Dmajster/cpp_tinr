#pragma once
#include "../rendering/Mesh.h"
#include "../rendering/Material.h"

struct RenderComponent
{
	Mesh* mesh;
	Material* material;
};
