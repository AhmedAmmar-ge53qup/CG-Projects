#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

// For Light
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

// For Assimp
uniform sampler2D texSampler1;
uniform sampler2D texSampler2;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;


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
	vec3 halfDir = normalize(lightDir + viewDir);
	float NDotH = max(dot(normal, halfDir), 0.0f);
	vec3 specular = lightColor * specularFactor * pow(NDotH, shininess);

	vec4 texel = texture(texture_diffuse1, TexCoord);
	fragColor = vec4((ambient + diffuse + specular), 1.0f) * texel;
}