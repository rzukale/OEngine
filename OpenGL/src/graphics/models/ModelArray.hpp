#pragma once

#include "../Model.h"

constexpr auto UPPER_BOUND = 100;

template <class T>
class ModelArray
{
public:
	std::vector<RigidBody> m_Instances;

	void Init()
	{
		m_Model.Init();

		// generate m_PositionVBO
		GLCall(glGenBuffers(1, &m_PositionVBO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_PositionVBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, UPPER_BOUND * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

		// generate m_SizeVBO
		GLCall(glGenBuffers(1, &m_SizeVBO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_SizeVBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, UPPER_BOUND * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

		// set attribute pointer for each mesh
		for (unsigned int i = 0, size = m_Model.m_Meshes.size(); i < size; i++)
		{
			GLCall(glBindVertexArray(m_Model.m_Meshes[i].m_VAO));
			// Positions
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_PositionVBO));
			GLCall(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
			GLCall(glEnableVertexAttribArray(3));
			// Sizes
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_SizeVBO));
			GLCall(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
			GLCall(glEnableVertexAttribArray(4));

			GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

			GLCall(glVertexAttribDivisor(3, 1)); // reset 3rd attribute every 1 instance
			GLCall(glVertexAttribDivisor(4, 1)); // reset 4th attribute every 1 instance

			GLCall(glBindVertexArray(0));
		}
	}
	void Render(Shader& shader, float DeltaTime, bool bSetList = true)
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
		m_Model.Render(shader, DeltaTime, false, false);

		int size = std::min(UPPER_BOUND, (int)m_Positions.size()); // if more than 100 instances, only render 100
		// update data
		if (m_Positions.empty() == false)
		{
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_PositionVBO));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size * 3 * sizeof(float), &m_Positions[0]));
			
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_SizeVBO));
			GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size * 3 * sizeof(float), &m_Sizes[0]));

			GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}

		// render instanced data
		for (unsigned int i = 0, length = m_Model.m_Meshes.size(); i < length; i++)
		{
			GLCall(glBindVertexArray(m_Model.m_Meshes[i].m_VAO));
			GLCall(glDrawElementsInstanced(GL_TRIANGLES, m_Model.m_Meshes[i].m_Indices.size(), GL_UNSIGNED_INT, 0, size));
			GLCall(glBindVertexArray(0));
		}
	}
	void SetSize(glm::vec3 Size)
	{
		m_Model.m_Size = Size;
	}
	void Cleanup()
	{
		m_Model.Cleanup();
	}

protected:
	T m_Model;

	unsigned int m_PositionVBO;
	unsigned int m_SizeVBO;
	std::vector<glm::vec3> m_Positions;
	std::vector<glm::vec3> m_Sizes;

};
