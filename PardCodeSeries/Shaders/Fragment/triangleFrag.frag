#version 460 core

layout(location = 0) in vec3 vertOutColor;

layout(location = 0) out vec4 FragColor;



void main()
{
	FragColor = vec4(vertOutColor, 1);
}