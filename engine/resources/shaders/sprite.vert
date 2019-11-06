#version 150

uniform mat4 m;
uniform mat4 vp;

in vec3 in_position;
in vec2 in_uv;
	
out vec2 frag_uv;

void main()
{
	frag_uv = in_uv;
	
	gl_Position = vp * m * vec4(in_position, 1.0);
}