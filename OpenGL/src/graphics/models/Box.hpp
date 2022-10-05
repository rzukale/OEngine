#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "../Shader.h"
#include "ModelArray.hpp"

class Box
{
public:

    std::vector<glm::vec3> m_Offsets;
    std::vector<glm::vec3> m_Sizes;

	void Init()
	{
        m_Vertices = {
            // position             x   y   z   i
             0.5f,  0.5f,  0.5f, // +   +   +   0
            -0.5f,  0.5f,  0.5f, // -   +   +   1
            -0.5f, -0.5f,  0.5f, // -   -   +   2
             0.5f, -0.5f,  0.5f, // +   -   +   3
             0.5f,  0.5f, -0.5f, // +   +   -   4
            -0.5f,  0.5f, -0.5f, // -   +   -   5
            -0.5f, -0.5f, -0.5f, // -   -   -   6
             0.5f, -0.5f, -0.5f  // +   -   -   7
        };
        m_Indices = { // 12 lines
            // front face (+ve z)
            0, 1,
            1, 2,
            2, 3,
            3, 0,
            // back face (-ve z)
            4, 5,
            5, 6,
            6, 7,
            7, 4,
            // right face (+ve x)
            0, 4,
            3, 7,
            // left face (-ve x)
            1, 5,
            2, 6
        };

        GLCall(glGenVertexArrays(1, &m_VAO));
        GLCall(glBindVertexArray(m_VAO));

        GLCall(glGenBuffers(1, &m_VBO));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
        GLCall(glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), &m_Vertices[0], GL_STATIC_DRAW));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

        // generate m_PositionVBO
        GLCall(glGenBuffers(1, &m_OffsetVBO));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_OffsetVBO));
        GLCall(glBufferData(GL_ARRAY_BUFFER, UPPER_BOUND * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

        // generate m_SizeVBO
        GLCall(glGenBuffers(1, &m_SizeVBO));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_SizeVBO));
        GLCall(glBufferData(GL_ARRAY_BUFFER, UPPER_BOUND * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));


        GLCall(glGenBuffers(1, &m_EBO));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW));

        // attrib pointers
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
        GLCall(glEnableVertexAttribArray(0));
        // Offsets
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_OffsetVBO));
        GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
        GLCall(glEnableVertexAttribArray(1));
        // Sizes
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_SizeVBO));
        GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
        GLCall(glEnableVertexAttribArray(2));

        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

        GLCall(glVertexAttribDivisor(1, 1)); // reset 1st attribute every 1 instance
        GLCall(glVertexAttribDivisor(2, 1)); // reset 2nd attribute every 1 instance

		GLCall(glBindVertexArray(0));
	}

    void Render(Shader& shader)
	{
        shader.SetMat4("model", glm::mat4(1.0f));

        int size = std::min(UPPER_BOUND, (int)m_Offsets.size()); // if more than 100 instances, only render 100
        // update data
        if (size != 0)
        {
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_OffsetVBO));
            GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size * 3 * sizeof(float), &m_Offsets[0]));

            GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_SizeVBO));
            GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size * 3 * sizeof(float), &m_Sizes[0]));

            GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        }

        // render instanced data
        GLCall(glBindVertexArray(m_VAO));
        GLCall(glDrawElementsInstanced(GL_LINES, m_Indices.size(), GL_UNSIGNED_INT, 0, size));
        GLCall(glBindVertexArray(0));
       
	}

    void Cleanup()
	{
        GLCall(glDeleteVertexArrays(1, &m_VAO));
        GLCall(glDeleteBuffers(1, &m_VBO));
        GLCall(glDeleteBuffers(1, &m_OffsetVBO));
        GLCall(glDeleteBuffers(1, &m_SizeVBO))
	}


private:
	unsigned int m_VAO, m_VBO, m_EBO, m_OffsetVBO, m_SizeVBO;

	std::vector<float> m_Vertices;
	std::vector<unsigned int> m_Indices;
};
