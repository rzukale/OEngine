#include "Bounds.h"
#include <algorithm>

BoundingRegion::BoundingRegion(BoundTypes Type)	: m_Type(Type) {}

BoundingRegion::BoundingRegion(glm::vec3 Center, float Radius) : m_Type(BoundTypes::SPHERE), m_Center(Center), m_Radius(Radius) {}

BoundingRegion::BoundingRegion(glm::vec3 Min, glm::vec3 Max) : m_Type(BoundTypes::AABB), m_Min(Min), m_Max(Max) {}

glm::vec3 BoundingRegion::CalculateCenter()
{
	return ((m_Type == BoundTypes::AABB) ? (m_Min + m_Max) / 2.0f : m_Center);
}

glm::vec3 BoundingRegion::CalculateDimensions()
{
	return ((m_Type == BoundTypes::AABB) ? (m_Max - m_Min) : glm::vec3(2.0f * m_Radius));
}

bool BoundingRegion::ContainsPoint(glm::vec3 Point)
{
	if (m_Type == BoundTypes::AABB)
	{
		// box - point must be larger than min and smaller than max
		return ((Point.x >= m_Min.x) && (Point.x <= m_Max.x) &&
			(Point.y >= m_Min.y) && (Point.y <= m_Max.y) &&
			(Point.z >= m_Min.z) && (Point.z <= m_Max.z));
	}
	float DistanceSquared = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		DistanceSquared += (m_Center[i] - Point[i]) * (m_Center[i] - Point[i]);
	}

	return (DistanceSquared <= (m_Radius * m_Radius));
}

bool BoundingRegion::ContainsRegion(BoundingRegion Region)
{
	if (Region.m_Type == BoundTypes::AABB)
	{
		// is a box, return if contains min and max
		return (ContainsPoint(Region.m_Min) && ContainsPoint(Region.m_Max));
	}
	else if (Region.m_Type == BoundTypes::SPHERE && m_Type == BoundTypes::SPHERE) // both are spheres
	{
		// if true, 1 sphere completely envelops the other sphere
		return ((glm::length(m_Center - Region.m_Center) + Region.m_Radius) < m_Radius);
	}

	if (!ContainsPoint(Region.m_Center))
	{
		// Center outside box, therefore visible
		return false;
	}
	for (int i = 0; i < 3; i++)
	{
		if ((abs(m_Max[i] - Region.m_Center[i]) < Region.m_Radius) ||
			(abs(Region.m_Center[i] - m_Min[i]) < Region.m_Radius))
		{
			return false;
		}
	}
	return true;
}

bool BoundingRegion::IntersectsWith(BoundingRegion Region)
{
	if (m_Type == BoundTypes::AABB && Region.m_Type == BoundTypes::AABB)
	{
		glm::vec3 Radius = CalculateDimensions() / 2.0f; // "radius"
		glm::vec3 RegionRadius = Region.CalculateDimensions() / 2.0f;

		glm::vec3 Center = CalculateCenter();
		glm::vec3 RegionCenter = Region.CalculateCenter();

		glm::vec3 Distance = abs(Center - RegionCenter);

		for (int i = 0; i < 3; i++)
		{
			if (Distance[i] > (Radius[i] + RegionRadius[i]))
			{
				return false; // no overlap on this axis
			}
		}
		return true;
	}
	else if (m_Type == BoundTypes::SPHERE && Region.m_Type == BoundTypes::SPHERE)
	{
		return (glm::length(m_Center - Region.m_Center) < (m_Radius + Region.m_Radius));
	}
	else if (m_Type == BoundTypes::SPHERE) // Region is AABB
	{
		float DistanceSquared = 0.0f;
		for (int i = 0; i < 3; i++) {
			// determine closest side
			float ClosestPoint = std::max(Region.m_Min[i], std::min(m_Center[i], Region.m_Max[i]));
			// add distance
			DistanceSquared += (ClosestPoint - m_Center[i]) * (ClosestPoint - m_Center[i]);
		}

		return DistanceSquared < (m_Radius * m_Radius);
	}
	// this is a box, Region is Sphere
	return Region.IntersectsWith(*this);
}
