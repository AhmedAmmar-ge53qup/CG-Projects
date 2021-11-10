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
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float NDotL = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = lightColor * NDotL;

	// Specular
	float specularFactor = 0.8f;
	float shininess = 40.0f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);	// negative is important to reflect in opposite direction
	float RDotV = max(dot(reflectDir, viewDir), 0.0f);
	vec3 specular = lightColor * pow(RDotV, shininess) * specularFactor;
	
	vec4 texel = texture(texture_map, TexCoord);
	fragColor = vec4((ambient + diffuse + specular), 1.0f) * texel;
}