#version 460 core

#define NR_POINT_LIGHTS 4

struct Material
{
	sampler2D textureDiffuse1;
	sampler2D textureSpecular1;
	sampler2D emmision;
	float shininess;
};

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadric;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 color;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 color;

	float cuttOff;
	float outerCutoff;

	bool enableXRay;
};

uniform vec3 viewPos;

// Structs
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

// Incoming Data
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

// Outgoing Data
out vec4 FragColor;

// Helper functions
vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{	
	vec3 nNormal = normalize(Normal);				// Normals for each fragment
	vec3 nViewDir = normalize(viewPos - FragPos);	// 
	
	// Emmision
	vec3 emmision = vec3(texture(material.emmision, TexCoords));

	vec3 emissionMask = step(vec3(1.0f), vec3(1.0f)-vec3(texture(material.textureSpecular1, TexCoords)));
	emmision = emmision * emissionMask;

	// vec3 result = CalcDirLight(dirLight, nNormal, nViewDir);
	
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		// result += CalcPointLight(pointLights[i], nNormal, FragPos, nViewDir);
	}

	vec3 result = CalcSpotLight(spotLight, nNormal, FragPos, nViewDir);

	FragColor = vec4(result, 1.0f); // Change 'result' to 'norm' for visualizing normals
}

vec3 CalcDirLight(DirLight dl, vec3 norm, vec3 viewDir)
{
	vec3 lightDir = normalize(-dl.direction);

	// Diffuse Shading
	float diff = max(dot(norm, lightDir), 0.0);

	// Specular Shading
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// Combine

	vec3 ambient = dl.ambient * vec3(texture(material.textureDiffuse1, TexCoords));
	vec3 diffuse = dl.diffuse * diff * vec3(texture(material.textureDiffuse1, TexCoords));
	vec3 specular = dl.specular * spec * vec3(texture(material.textureSpecular1, TexCoords));

	return (ambient+diffuse+specular);
}

vec3 CalcPointLight(PointLight pl, vec3 norm, vec3 fragPos, vec3 viewDir)
{

	vec3 lightDir = normalize(pl.position - fragPos);
	
	// Diffuse Lighting
	float diff = max(dot(norm, lightDir), 0.0f);
	
	// Specular Lighting
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	// Attenuation
	float dist = length(pl.position - fragPos);
	float attenuation = 1.0/(pl.constant + (pl.linear * dist) + (pl.quadric * (dist * dist)));


	vec3 ambient = vec3(texture(material.textureDiffuse1, TexCoords)) * pl.ambient;
	vec3 diffuse = vec3(texture(material.textureDiffuse1, TexCoords)) * diff * pl.diffuse;
	vec3 specular = vec3(texture(material.textureSpecular1, TexCoords)) * spec * pl.specular;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular) * pl.color;
}

vec3 CalcSpotLight(SpotLight sl, vec3 norm, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(sl.position - fragPos);
	
	// Diffuse Lighting
	float diff = max(dot(norm, lightDir), 0.0f);
	
	// Specular Lighting
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	float theta = dot(lightDir, normalize(-sl.direction));
	float epsilon = spotLight.cuttOff - sl.outerCutoff;
	float intensity = clamp((theta - sl.outerCutoff) / epsilon, 0.0f, 1.0f);

	// X-ray effect
	if(spotLight.enableXRay)
	{
		float coeff = dot(norm, lightDir);

		if(coeff >= 0.0f && coeff <= 1.0f)
		{
			float spot = dot(lightDir, normalize(-spotLight.direction));

			if(spot >= spotLight.cuttOff)
			{
				discard;
			}
		}
	}

	vec3 ambient = vec3(texture(material.textureDiffuse1, TexCoords)) * sl.ambient;
	vec3 diffuse = vec3(texture(material.textureDiffuse1, TexCoords)) * diff * sl.diffuse;
	vec3 specular = vec3(texture(material.textureSpecular1, TexCoords)) * spec * sl.specular;

	diffuse *= intensity;
	specular *= intensity;

	return (ambient + diffuse + specular) * sl.color;
}