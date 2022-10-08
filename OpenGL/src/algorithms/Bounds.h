#pragma once

#include <glm/glm.hpp>

enum class BoundTypes : unsigned char
{
	AABB = 0x00, // axis-aligned bounding box
	SPHERE = 0x01
};


class BoundingRegion
{
public:
	BoundTypes m_Type;

	// sphere values
	glm::vec3 m_Center;
	float m_Radius;

	// bounding box values
	glm::vec3 m_Min;
	glm::vec3 m_Max;

	BoundingRegion(BoundTypes Type);
	BoundingRegion(glm::vec3 Center, float Radius);
	BoundingRegion(glm::vec3 Min, glm::vec3 Max);

	glm::vec3 CalculateCenter();
	glm::vec3 CalculateDimensions();

	bool ContainsPoint(glm::vec3 Point);
	// Returns true if Region completely inside evaluated Region
	bool ContainsRegion(BoundingRegion Region);
	// Returns true if Region partially inside evaluated Region
	bool IntersectsWith(BoundingRegion Region);
};

