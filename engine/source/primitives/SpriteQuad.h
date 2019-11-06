#pragma once
#include "../rendering/Mesh.h"
#include "SpriteStrip.h"

class SpriteQuad : public Mesh
{
public:
	explicit SpriteQuad(const SpriteData t_sprite_data)
	{
		vertices = {
			-1.0f, -1.0f, +0.0f, t_sprite_data.u_start, t_sprite_data.v_start,
			-1.0f, +1.0f, +0.0f, t_sprite_data.u_start, t_sprite_data.v_end,
			+1.0f, +1.0f, +0.0f, t_sprite_data.u_end, t_sprite_data.v_end,
			+1.0f, -1.0f, +0.0f, t_sprite_data.u_end, t_sprite_data.v_start,
		};

		indices = {
			0, 1, 2,
			2, 3, 0,
		};

		layout = {
			{ "in_position", 3 },
			{ "in_uv", 2 }
		};
	}
};
