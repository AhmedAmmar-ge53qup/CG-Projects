#version 330 core

out vec4 fragColor;

in vec2 TexCoord;

uniform vec4 vertColor;
uniform sampler2D texSampler1;
uniform sampler2D texSampler2;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;

void main()
{
	fragColor = texture(texture_diffuse1, TexCoord);
}