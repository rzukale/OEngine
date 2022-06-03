#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 Color;

void main()
{
	gl_Position = vec4(aPos, 1.0);

	if (aPos.x < 0.0)
	{
		Color = vec4(1.0, 0.0, 0.0, 1.0);
	}
	else if (aPos.x == 0)
	{
		Color = vec4(0.0, 1.0, 0.0, 1.0);
	}
	else
	{
		Color = vec4(0.0, 0.0, 1.0, 1.0);
	}
}