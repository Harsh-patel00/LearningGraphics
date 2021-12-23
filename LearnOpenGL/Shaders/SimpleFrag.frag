#version 460 core

in vec2 TexCoords;

uniform sampler2D texture1;

out vec4 FragColor;

float near = 1.0;
float far = 100.0;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0f - 1.0f;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
	// FragColor = vec4(1.0f);

	FragColor = texture(texture1, TexCoords);
	
	// float depth = LinearizeDepth(gl_FragCoord.z) / far;

	/// FragColor = vec4(vec3(gl_FragCoord.z), 1.0f);
}