#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "../Shader.h"
#include "../../algorithms/Bounds.h"
#include "../glmemory.hpp"

#define UPPER_BOUND 100

class Box
{
public:

    std::vector<glm::vec3> m_Positions;
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

        m_VAO.Generate();
        m_VAO.Bind();

        m_VAO["EBO"] = BufferObject(GL_ELEMENT_ARRAY_BUFFER);
        m_VAO["EBO"].Generate();
        m_VAO["EBO"].Bind();
		m_VAO["EBO"].SetData<GLuint>((GLuint)m_Indices.size(), &m_Indices[0], GL_STATIC_DRAW);

        m_VAO["VBO"] = BufferObject(GL_ARRAY_BUFFER);
        m_VAO["VBO"].Generate();
        m_VAO["VBO"].Bind();
        m_VAO["VBO"].SetData<GLfloat>((GLuint)m_Vertices.size(), &m_Vertices[0], GL_STATIC_DRAW);
        m_VAO["VBO"].SetAttribPointer<GLfloat>(0, 3, GL_FLOAT, 3, 0);

        m_VAO["posVBO"] = BufferObject(GL_ARRAY_BUFFER);
        m_VAO["posVBO"].Generate();
        m_VAO["posVBO"].Bind();
        m_VAO["posVBO"].SetData<glm::vec3>(UPPER_BOUND, NULL, GL_DYNAMIC_DRAW);
        m_VAO["posVBO"].SetAttribPointer<glm::vec3>(1, 3, GL_FLOAT, 1, 0, 1);

        m_VAO["sizeVBO"] = BufferObject(GL_ARRAY_BUFFER);
        m_VAO["sizeVBO"].Generate();
        m_VAO["sizeVBO"].Bind();
        m_VAO["sizeVBO"].SetData<glm::vec3>(UPPER_BOUND, NULL, GL_DYNAMIC_DRAW);
        m_VAO["sizeVBO"].SetAttribPointer<glm::vec3>(2, 3, GL_FLOAT, 1, 0, 1);
        m_VAO["sizeVBO"].Unbind();

        ArrayObject::Unbind();
	}

    void Render(Shader& shader)
	{
        shader.SetMat4("model", glm::mat4(1.0f));

        int instances = std::min(UPPER_BOUND, (int)m_Positions.size());
        if (instances != 0)
        {
            m_VAO["posVBO"].Bind();
            m_VAO["posVBO"].UpdateData<glm::vec3>(0, instances, &m_Positions[0]);

            m_VAO["sizeVBO"].Bind();
            m_VAO["sizeVBO"].UpdateData<glm::vec3>(0, instances, &m_Sizes[0]);
        }
        // render instanced data
        m_VAO.Bind();
        m_VAO.Draw(GL_LINES, (GLuint)m_Indices.size(), GL_UNSIGNED_INT, 0, instances);
        ArrayObject::Unbind();
	}

    void AddInstance(BoundingRegion Region, glm::vec3 Position, glm::vec3 Size)
	{
        m_Positions.push_back(Region.CalculateCenter() * Size + Position);

        m_Sizes.push_back(Region.CalculateDimensions() * Size);
	}

    void Cleanup()
	{
        m_VAO.Cleanup();
	}


private:
    ArrayObject m_VAO;

	std::vector<float> m_Vertices;
	std::vector<unsigned int> m_Indices;
};
