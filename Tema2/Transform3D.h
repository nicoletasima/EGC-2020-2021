#pragma once

#include <include/glm.h>

namespace Transform3D
{
	// Translate matrix
	inline glm::mat4 Translate(float translateX, float translateY, float translateZ)
	{
		// implement translate matrix
		return glm::transpose(glm::mat4(1, 0, 0, translateX, 
										0, 1, 0, translateY, 
										0, 0, 1, translateZ, 
										0, 0, 0, 1));
	}

	// Scale matrix
	inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ)
	{
		// implement scale matrix
		return glm::transpose(glm::mat4(scaleX, 0.f, 0.f, 0.f,
										0.f, scaleY, 0.f, 0.f,
										0.f, 0.f, scaleZ, 0.f, 
										0.f, 0.f, 0.f, 1.f));
	}

	// Rotate matrix relative to the OZ axis
	inline glm::mat4 RotateOZ(float radians)
	{
		// implement rotate matrix
		return glm::transpose(glm::mat4(cos(radians), -sin(radians), 0.f, 0.f,
										sin(radians), cos(radians), 0.f, 0.f, 
										0.f, 0.f, 1.f, 0.f,
										0.f, 0.f, 0.f, 1.f));
	}

	// Rotate matrix relative to the OY axis
	inline glm::mat4 RotateOY(float radians)
	{
		// implement rotate matrix
		return glm::transpose(glm::mat4(cos(radians), 0.f, sin(radians), 0.f, 
										0.f, 1.f, 0.f, 0.f, 
										-sin(radians), 0.f, cos(radians), 0.f, 
										0.f, 0.f, 0.f, 1.f));
	}

	// Rotate matrix relative to the OX axis
	inline glm::mat4 RotateOX(float radians)
	{
		// implement rotate matrix
		return glm::transpose(glm::mat4(1.f, 0.f, 0.f, 0.f, 
										0.f, cos(radians), -sin(radians), 0.f, 
										0.f, sin(radians), cos(radians), 0.f, 
										0.f, 0.f, 0.f, 1.f));
	}
}
