#version 150

in vec2 frag_uv;
in vec3 frag_normal;
	
out vec4 out_color;
	
void main()
{
	vec3 norm = normalize(frag_normal);
	vec3 lightDir = normalize(vec3(-1.0, 1.0, 0));  
	float diffuse = max(dot(norm, lightDir), 0.0);
	
	out_color = vec4(diffuse * vec3(0.1, 0.7, 0.1), 1.0);
}