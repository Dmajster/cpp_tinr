#version 150
	
uniform mat4 m;
uniform mat4 vp;

in vec3 vert_position;
in vec3 vert_normal;
in vec2 vert_uv;
	
out vec2 frag_uv;
out vec3 frag_normal;

void main()
{
	frag_uv = vert_uv;
	frag_normal = vert_normal;
	
	gl_Position = vp * m * vec4(vert_position, 1.0);
}