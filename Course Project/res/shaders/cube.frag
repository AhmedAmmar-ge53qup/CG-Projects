#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

// For Light
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 color;

uniform sampler2D texSampler;
uniform bool isTextured = false;

out vec4 fragColor;
void main()
{
	// Ambient
	float ambientFactor = 0.4f;
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

	vec4 texel = texture(texSampler, TexCoord);
	vec3 result = (ambient + diffuse + specular) * color;

	if (isTextured)
		fragColor = vec4((ambient + diffuse + specular), 1.0) * texel;
	else
		fragColor = vec4(result, 1.0);
}