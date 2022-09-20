#include "Light.h"

void PointLight::Render(Shader& shader, int Index)
{
	std::string Name = "pointLights[" + std::to_string(Index) + "]";

	shader.Set3Float(Name + ".position", m_Position);
	shader.Set4Float(Name + ".ambient", m_Ambient);
	shader.Set4Float(Name + ".diffuse", m_Diffuse);
	shader.Set4Float(Name + ".specular", m_Specular);

	shader.SetFloat(Name + ".k0", m_k0);
	shader.SetFloat(Name + ".k1", m_k1);
	shader.SetFloat(Name + ".k2", m_k2);

}

void DirectionalLight::Render(Shader& shader)
{
	std::string Name = "directionalLight";

	shader.Set3Float(Name + ".direction", m_Direction);
	shader.Set4Float(Name + ".ambient", m_Ambient);
	shader.Set4Float(Name + ".diffuse", m_Diffuse);
	shader.Set4Float(Name + ".specular", m_Specular);
}

void SpotLight::Render(Shader& shader, int Index)
{
	std::string Name = "spotLights[" + std::to_string(Index) + "]";

	shader.Set3Float(Name + ".position", m_Position);
	shader.Set3Float(Name + ".direction", m_Direction);
	shader.Set4Float(Name + ".ambient", m_Ambient);
	shader.Set4Float(Name + ".diffuse", m_Diffuse);
	shader.Set4Float(Name + ".specular", m_Specular);

	shader.SetFloat(Name + ".cutOff", m_CutOff);
	shader.SetFloat(Name + ".outerCutOff", m_OuterCutOff);

	shader.SetFloat(Name + ".k0", m_k0);
	shader.SetFloat(Name + ".k1", m_k1);
	shader.SetFloat(Name + ".k2", m_k2);
}
