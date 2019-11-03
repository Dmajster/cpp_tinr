#version 150

in vec2 vert_position;
in vec2 vert_uv;

out vec2 frag_uv;

void main()
{
	frag_uv = vert_uv;
	gl_Position = vec4(vert_position, 0.0f, 1.0f);
}