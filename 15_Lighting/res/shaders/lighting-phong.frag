#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D texture_map;

out vec4 fragColor;

void main()
{
	// Ambient
	float ambientFactor = 0.2f;
	vec3 ambient = lightColor * ambientFactor;
	// Diffuse
	vec3 diffuse = vec3(0.0f);
	// Specular
	vec3 specular = vec3(0.0f);
	
	vec4 texel = texture(texture_map, TexCoord);
	fragColor = vec4((ambient + diffuse + specular), 1.0f) * texel;
}