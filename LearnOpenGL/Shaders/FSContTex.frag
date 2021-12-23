#version 460 core

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform bool useTexture;

in vec2 TexCoords;

out vec4 FragColor;

// Upper Triangle
void main()
{	

	if(useTexture)
	{
		// Use this line to use texture
		FragColor = mix(texture(texture1, TexCoords), texture(texture2, TexCoords), 0.2f);
	}
	else
	{
		// Draw black color cube if not using the texture and using this shader
		FragColor = vec4(0.0f);
	}
}