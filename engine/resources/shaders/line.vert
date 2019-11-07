#version 150

uniform mat4 m;
uniform mat4 vp;

in vec3 in_position;

void main()
{
	gl_Position = vp * m * vec4(in_position, 1.0);
}