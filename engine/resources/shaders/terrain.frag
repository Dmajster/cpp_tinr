#version 150

in vec2 frag_uv;
in vec3 frag_normal;
	
out vec4 out_color;
	
void main()
{
	vec3 norm = normalize(frag_normal);
	vec3 lightDir = normalize(vec3(-1.0f, 1.0f, 0f));  
	float diffuse = max(dot(norm, lightDir), 0.0f);
	
	out_color = diffuse * vec4(0.1f, 0.7f, 0.1f, 1.0f);
}