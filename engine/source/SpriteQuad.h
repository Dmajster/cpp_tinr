#pragma once
#include "Mesh.h"
#include "SpriteStrip.h"

class SpriteQuad : public Mesh
{
public:
	explicit SpriteQuad(const SpriteData t_sprite_data)
	{
		positions = {
			-1.0f, -1.0f,
			-1.0f, +1.0f,
			+1.0f, +1.0f,
			+1.0f, -1.0f,
		};

		uvs = {
			t_sprite_data.u_start, t_sprite_data.v_start,
			t_sprite_data.u_start, t_sprite_data.v_end,
			t_sprite_data.u_end, t_sprite_data.v_end,
			t_sprite_data.u_end, t_sprite_data.v_start,
		};
		
		indices = {
			0, 1, 2,
			2, 3, 0,
		};
	}
};
