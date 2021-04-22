#version 410 core
out vec4 FragColor;

in vec3 worldPos;

uniform vec3 inputColor;

void main()
{
	FragColor = vec4(worldPos, 1.0);
}