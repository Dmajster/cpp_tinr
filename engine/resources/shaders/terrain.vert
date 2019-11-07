#version 150
	
uniform mat4 m;
uniform mat4 vp;

in vec3 in_position;
in vec3 in_normal;
	
out vec3 frag_normal;

void main()
{
	frag_normal = in_normal;
	
	gl_Position = vp * m * vec4(in_position, 1.0);
}