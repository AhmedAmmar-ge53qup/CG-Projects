#version 330 core

out vec4 fragColor;
out vec3 fragColor2;

in vec2 TexCoord;

uniform vec4 vertColor;
uniform sampler2D texSampler2;
uniform sampler2D texSampler3;

void main()
{
	//fragColor = vertColor;
	//fragColor = texture(texSampler1, TexCoord);
	//fragColor = mix(texture(texSampler1, TexCoord), texture(texSampler2, TexCoord), 0.4);

	vec4 backgroundTexColor = texture(texSampler2, TexCoord);
	vec4 marioTexColor = texture(texSampler3, TexCoord);
	fragColor2 = mix(backgroundTexColor.rgb, marioTexColor.rgb, marioTexColor.a);
}