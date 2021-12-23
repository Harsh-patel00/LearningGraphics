#version 460 core

uniform vec3 lightColor;

out vec4 FragColorL;

void main()
{	
	FragColorL = vec4(lightColor, 1.0f);
}