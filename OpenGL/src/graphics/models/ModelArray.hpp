#pragma once

#include "../Model.h"
#include "Box.hpp"

#define UPPER_BOUND 100

template <class T>
class ModelArray
{
public:
	std::vector<RigidBody> m_Instances;

	void Init()
	{
		m_Model.Init();

		m_PositionVBO = BufferObject(GL_ARRAY_BUFFER);
		m_PositionVBO.Generate();
		m_PositionVBO.Bind();
		m_PositionVBO.SetData<glm::vec3>(UPPER_BOUND, NULL, GL_DYNAMIC_DRAW);

		m_SizeVBO = BufferObject(GL_ARRAY_BUFFER);
		m_SizeVBO.Generate();
		m_SizeVBO.Bind();
		m_SizeVBO.SetData<glm::vec3>(UPPER_BOUND, nullptr, GL_DYNAMIC_DRAW);

		// set attribute pointer for each mesh
		for (unsigned int i = 0, size = (unsigned int)m_Model.m_Meshes.size(); i < size; i++)
		{
			m_Model.m_Meshes[i].m_VAO.Bind();
			m_PositionVBO.Bind();
			m_PositionVBO.SetAttribPointer<glm::vec3>(3, 3, GL_FLOAT, 1, 0, 1);

			m_SizeVBO.Bind();
			m_SizeVBO.SetAttribPointer<glm::vec3>(4, 3, GL_FLOAT, 1, 0, 1);

			ArrayObject::Unbind();
		}
	}
	void Render(Shader& shader, float DeltaTime, Box* box, bool bSetList = true)
	{
		if (bSetList)
		{
			m_Positions.clear();
			m_Sizes.clear();

			for (RigidBody& rigidbody : m_Instances)
			{
				rigidbody.Update(DeltaTime);
				m_Positions.push_back(rigidbody.m_Position);
				m_Sizes.push_back(m_Model.m_Size);
			}
		}

		shader.SetMat4("model", glm::mat4(1.0f));
		m_Model.Render(shader, DeltaTime, nullptr, false, false);

		unsigned int instances = std::min(UPPER_BOUND, (int)m_Positions.size());
		if (instances != 0)
		{
			m_PositionVBO.Bind();
			m_PositionVBO.UpdateData<glm::vec3>(0, instances, &m_Positions[0]);

			m_SizeVBO.Bind();
			m_SizeVBO.UpdateData<glm::vec3>(0, instances, &m_Sizes[0]);

			m_SizeVBO.Unbind();
		}

		// render instanced data
		for (unsigned int i = 0, length = (unsigned int)m_Model.m_Meshes.size(); i < length; i++)
		{
			for (unsigned int j = 0; j < instances; j++)
			{
				box->AddInstance(m_Model.m_Meshes[i].m_BoundingRegion, m_Positions[j], m_Sizes[j]);
			}
			m_Model.m_Meshes[i].m_VAO.Bind();
			m_Model.m_Meshes[i].m_VAO.Draw(GL_TRIANGLES, (GLuint)m_Model.m_Meshes[i].m_Indices.size(), GL_UNSIGNED_INT, 0, instances);
			ArrayObject::Unbind();
		}
	}
	void SetSize(glm::vec3 Size)
	{
		m_Model.m_Size = Size;
	}
	void Cleanup()
	{
		m_Model.Cleanup();
		m_PositionVBO.Cleanup();
		m_SizeVBO.Cleanup();
	}

protected:
	T m_Model;

	BufferObject m_PositionVBO;
	BufferObject m_SizeVBO;
	std::vector<glm::vec3> m_Positions;
	std::vector<glm::vec3> m_Sizes;

};
