#version 460 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool useTexture;
uniform bool useMVP;


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

void main()
{

	if(useMVP)
	{
		// Transformed position
		gl_Position = projection * view * model * vec4(aPos, 1.0f);
	}
	else
	{
		// Predefined position
		gl_Position = vec4(aPos, 1.0f);
	}

	// Calculate all fragment positions
	FragPos = vec3((model) * vec4(aPos, 1.0f));
	
	// Calculate vertex normals, for diffuse and specular lighting
	Normal = mat3(transpose(inverse(model))) * aNormal; // Normal Matrix

	// Set output texture coordinates
	if(useTexture)
		TexCoords = aTexCoords;
}