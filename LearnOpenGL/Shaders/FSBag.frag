#version 460 core

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform DirLight dirLight;
uniform sampler2D textureDiffuse1;
uniform sampler2D textureSpecular1;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir);

void main()
{

	vec3 nNormal = normalize(Normal);				// Normals for each fragment
	vec3 nViewDir = normalize(viewPos - FragPos);	// 

	vec3 result = CalcDirLight(dirLight, nNormal, nViewDir);

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight dl, vec3 norm, vec3 viewDir)
{
	vec3 lightDir = normalize(-dl.direction);

	// Diffuse Shading
	float diff = max(dot(norm, lightDir), 0.0);

	// Specular Shading
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	// Combine
	vec3 ambient = dl.ambient * vec3(texture(textureDiffuse1, TexCoords));
	vec3 diffuse = dl.diffuse * diff * vec3(texture(textureDiffuse1, TexCoords));
	vec3 specular = dl.specular * spec * vec3(texture(textureSpecular1, TexCoords));

	return (ambient+diffuse+specular);
}