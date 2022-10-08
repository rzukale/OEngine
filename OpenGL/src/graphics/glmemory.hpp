#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include "../error.h"

/*
 * Class for buffer objects
 * VBOs, EBOs etc.
 */

class BufferObject
{
public:

	BufferObject() : m_Value(0), m_Type(0) {}
	BufferObject(GLenum Type) : m_Value(0), m_Type(Type) {}

	void Generate()
	{
		GLCall(glGenBuffers(1, &m_Value));
	}

	void Bind()
	{
		GLCall(glBindBuffer(m_Type, m_Value));
	}

	template<typename T>
	void SetData(GLuint NumberOfElements, T* Data, GLenum Usage)
	{
		GLCall(glBufferData(m_Type, NumberOfElements * sizeof(T), Data, Usage));
	}

	template<typename T>
	void UpdateData(GLintptr Offset, GLuint NumberOfElements, T* Data)
	{
		GLCall(glBufferSubData(m_Type, Offset, NumberOfElements * sizeof(T), Data));
	}

	template<typename T>
	void SetAttribPointer(GLuint Index, GLint Size, GLenum Type, GLuint Stride, GLuint Offset, GLuint Divisor = 0)
	{
		GLCall(glVertexAttribPointer(Index, Size, Type, GL_FALSE, Stride * sizeof(T), (void*)(Offset * sizeof(T))));
		GLCall(glEnableVertexAttribArray(Index));
		if (Divisor > 0)
		{
			// reset Index attribute every Divisor iteration (instancing)
			GLCall(glVertexAttribDivisor(Index, Divisor));
		}
	}

	void Unbind()
	{
		GLCall(glBindBuffer(m_Type, 0));
	}

	void Cleanup()
	{
		GLCall(glDeleteBuffers(1, &m_Value));
	}

private:
	GLuint m_Value;
	GLenum m_Type;
};

/*
 *  Class for Array objects
 *	VAO
 */
class ArrayObject
{
public:

	ArrayObject() : m_Value(0) {}

	// get buffer reference
	BufferObject& operator[](const char* key)
	{
		return m_Buffers[key];
	}

	void Generate()
	{
		GLCall(glGenVertexArrays(1, &m_Value));
	}

	void Bind()
	{
		GLCall(glBindVertexArray(m_Value));
	}

	void Draw(GLenum Mode, GLuint Count, GLenum Type, GLint Indices, GLuint InstanceCount = 1)
	{
		GLCall(glDrawElementsInstanced(Mode, Count, Type, (void*)Indices, InstanceCount));
	}

	void Cleanup()
	{
		GLCall(glDeleteVertexArrays(1, &m_Value));
		for (auto& pair : m_Buffers)
		{
			pair.second.Cleanup();
		}
	}

	static void Unbind()
	{
		GLCall(glBindVertexArray(0));
	}

private:
	// map of names to buffers
	std::map<const char*, BufferObject> m_Buffers;
	GLuint m_Value;
};