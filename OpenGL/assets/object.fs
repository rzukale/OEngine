#version 330 core

struct Material
{
	vec4 diffuse;
	vec4 specular;
	float shininess;
};
uniform sampler2D diffuse0;
uniform sampler2D specular0;

#define MAX_SPOTLIGHTS 5

struct SpotLight
{
	vec3 position;
	vec3 direction;

	float cutOff;
	float outerCutOff;

	float k0;
	float k1;
	float k2;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

};
uniform SpotLight spotLights[MAX_SPOTLIGHTS];
uniform int NumberOfSpotlights;

struct DirectionalLight
{
	vec3 direction;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};
uniform DirectionalLight directionalLight;

#define MAX_POINTLIGHTS 20
struct PointLight
{
	vec3 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	float k0;
	float k1;
	float k2;
};
uniform PointLight pointLights[MAX_POINTLIGHTS];
uniform int NumberOfPointlights;

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 ViewPos;
uniform Material material;
uniform int NoTex;

vec4 CalculateDirectionalLight(vec3 Norm, vec3 ViewDirection, vec4 DiffMap, vec4 SpecMap);
vec4 CalculatePointlight(int Index, vec3 Norm, vec3 ViewDirection, vec4 DiffMap, vec4 SpecMap);
vec4 CalculateSpotlight(int Index, vec3 Norm, vec3 ViewDirection, vec4 DiffMap, vec4 SpecMap);

void main()
{
	vec3 Norm = normalize(Normal);
	vec3 ViewDirection = normalize(ViewPos - FragPos);

	vec4 DiffMap;
	vec4 SpecMap;

	if (NoTex == 1)
	{
		DiffMap = material.diffuse;
		SpecMap = material.specular;
	}
	else
	{
		DiffMap = texture(diffuse0, TexCoord);
		SpecMap = texture(specular0, TexCoord);
	}

	vec4 Result;

	// directional light
	Result = CalculateDirectionalLight(Norm, ViewDirection, DiffMap, SpecMap);

	// point lights
	for (int i = 0; i < NumberOfPointlights; i++)
	{
		Result += CalculatePointlight(i, Norm, ViewDirection, DiffMap, SpecMap);
	}

	// spot lights
	for (int i = 0; i < NumberOfSpotlights; i++)
	{
		Result += CalculateSpotlight(i, Norm, ViewDirection, DiffMap, SpecMap);
	}
	FragColor = Result;
}

vec4 CalculateSpotlight(int Index, vec3 Norm, vec3 ViewDirection, vec4 DiffMap, vec4 SpecMap)
{
	vec3 LightDirection = normalize(spotLights[Index].position - FragPos);

	float Theta = dot(LightDirection, normalize(-spotLights[Index].direction));

	vec4 Ambient = spotLights[Index].ambient * DiffMap;

	if (Theta > spotLights[Index].outerCutOff)
	{
		float Diff = max(dot(Norm, LightDirection), 0.0f);
		vec4 Diffuse = spotLights[Index].diffuse * (Diff * DiffMap);

		vec3 ReflectionDirection = reflect(-LightDirection, Norm);
		float Spec = pow(max(dot(ViewDirection, ReflectionDirection), 0.0f), material.shininess * 128);
		vec4 Specular = spotLights[Index].specular * (Spec * SpecMap);
	
		float Intensity = (Theta - spotLights[Index].outerCutOff) / (spotLights[Index].cutOff - spotLights[Index].outerCutOff);
		Intensity = clamp(Intensity, 0.0, 1.0);
		Diffuse *= Intensity;
		Specular *= Intensity;

		float Dist = length(spotLights[Index].position - FragPos);
		float Attenuation = 1.0 / (spotLights[Index].k0 + spotLights[Index].k1 * Dist + spotLights[Index].k2 * (Dist * Dist));
		
		return vec4(Ambient + Diffuse + Specular) * Attenuation;
	}
	return Ambient;
}

vec4 CalculateDirectionalLight(vec3 Norm, vec3 ViewDirection, vec4 DiffMap, vec4 SpecMap)
{
	// ambient
	vec4 Ambient = directionalLight.ambient * DiffMap;

	// diffuse
	vec3 lightDirection = normalize(-directionalLight.direction);
	float Diff = max(dot(Norm, lightDirection), 0.0);
	vec4 Diffuse = directionalLight.diffuse * (Diff * DiffMap);

	// Specular
	vec3 ReflectionDirection = reflect(-lightDirection, Norm);
	float Spec = pow(max(dot(ViewDirection, ReflectionDirection), 0.0), material.shininess * 128);
	vec4 Specular = directionalLight.specular * (Spec * SpecMap);

	return vec4(Ambient + Diffuse + Specular);
}

vec4 CalculatePointlight(int Index, vec3 Norm, vec3 ViewDirection, vec4 DiffMap, vec4 SpecMap)
{
	// ambient
	vec4 Ambient = pointLights[Index].ambient * DiffMap;

	// diffuse
	vec3 LightDirection = normalize(pointLights[Index].position - FragPos);
	float Diff = max(dot(Norm, LightDirection), 0.0);
	vec4 Diffuse = pointLights[Index].diffuse * (Diff * DiffMap);

	// specular
	vec3 ReflectionDirection = reflect(-LightDirection, Norm);
	float Spec = pow(max(dot(ViewDirection, ReflectionDirection), 0.0), material.shininess * 128);
	vec4 Specular = pointLights[Index].specular * (Spec * SpecMap);

	float Dist = length(pointLights[Index].position - FragPos);
	float Attenuation = 1.0 / (pointLights[Index].k0 + pointLights[Index].k1 * Dist + pointLights[Index].k2 * (Dist * Dist));

	return vec4(Ambient + Diffuse + Specular) * Attenuation;
}