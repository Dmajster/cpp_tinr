#version 150

uniform sampler2D sprite;

in vec2 frag_uv;
	
out vec4 out_color;

void main()
{
	out_color = vec4(0.0,0.0,0.0,1.0);
}