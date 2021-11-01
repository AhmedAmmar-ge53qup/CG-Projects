#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

out vec3 vertColor;

uniform mat4 model, view, projection;

void main()
{
	gl_Position = projection * view * model * vec4(pos.x, pos.y, pos.z, 1.0);
	vertColor = color;
}