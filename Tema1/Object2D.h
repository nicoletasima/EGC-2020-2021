#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

	// Create triangle
	Mesh* CreateTriangle(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

	// Create Bow
	Mesh* CreateBow(std::string name, glm::vec3 origin, glm::vec3 color);

	// Create Arrow
	Mesh* CreateArrowHead(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill);

	// Create Arrow Tail
	Mesh* CreateArrowTail(std::string name, glm::vec3 start, float length, glm::vec3 color);

	// Create Balloon
	Mesh* CreateBalloon(std::string name, glm::vec3 center, float R, glm::vec3 color, bool fill);

	// Create Shuriken
	Mesh* CreateShuriken(std::string name, glm::vec3 startCorner, float length, glm::vec3 color, bool fill);

	// Create Balloon Line
	Mesh* CreateLine(std::string name, glm::vec3 center, float R, glm::vec3 color);

	// Create Balloon Traingle
	Mesh* CreateBalloonTriangle(std::string, glm::vec3 center, float R, glm::vec3 color, bool fill);

	// Create PowerBar
	Mesh* CreatePowerBar(std::string, glm::vec3 leftBottomCorner, float width, float powerLength, glm::vec3 color, bool fill);

	// Create Life
	Mesh* CreateLife(std::string, glm::vec3 center, float R, glm::vec3 color, bool fill);
}

