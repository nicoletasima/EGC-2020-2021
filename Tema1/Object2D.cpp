#include "Object2D.h"

#include <Core/Engine.h>

Mesh* Object2D::CreatePowerBar(std::string name, glm::vec3 leftBottomCorner, float width, float powerLength, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices = 
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(powerLength, 0, 0), color),
		VertexFormat(corner + glm::vec3(powerLength, width, 0), color),
		VertexFormat(corner + glm::vec3(0, width, 0), color)
	};

	Mesh* powerBar = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		powerBar->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	powerBar->InitFromData(vertices, indices);
	return powerBar;
}

Mesh* Object2D::CreateBow(std::string name, glm::vec3 origin, glm::vec3 color)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	float u = 3 * 3.14 / 2;

	for (int i = 0; i < 100; i++) {
		vertices.push_back(VertexFormat(origin + glm::vec3(50 * cos(u), 50 * sin(u), 0), glm::vec3(1, 0, 0)));
		indices.push_back(i);
		u = u + 3.14 / 100;
	}
	vertices.push_back(VertexFormat(glm::vec3(50 * cos(3.14 / 2), 50 * sin(3.14 / 2), 0), color));
	indices.push_back(100);

	Mesh* bow = new Mesh(name);
	bow->SetDrawMode(GL_LINE_LOOP);
	bow->InitFromData(vertices, indices);
	return bow;
}

Mesh* Object2D::CreateArrowHead(std::string name, glm::vec3 start, float length, glm::vec3 color, bool fill) {
	
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices = {0, 1, 2, 3};


	vertices.push_back(VertexFormat(start + glm::vec3(length, 0, 0), color));
	vertices.push_back(VertexFormat(start + glm::vec3(length, -18, 0), color));		// 1
	vertices.push_back(VertexFormat(start + glm::vec3(length + 18, 0, 0), color));				// 2
	vertices.push_back(VertexFormat(start + glm::vec3(length, 18, 0), color)); // 3

	Mesh* arrowHead = new Mesh(name);
	if (!fill) {
		arrowHead->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		indices.push_back(0);
		indices.push_back(2);
		
	}

	arrowHead->InitFromData(vertices, indices);
	return arrowHead;

}

Mesh* Object2D::CreateArrowTail(std::string name, glm::vec3 start, float length, glm::vec3 color)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices = { 0, 1 };

	vertices.push_back(VertexFormat(start, color));
	vertices.push_back(VertexFormat(start + glm::vec3(length, 0, 0), color));

	Mesh* arrowTail = new Mesh(name);
	arrowTail->SetDrawMode(GL_LINES);
	arrowTail->InitFromData(vertices, indices);
	return arrowTail;
}

Mesh* Object2D::CreateBalloon(std::string name, glm::vec3 center, float R, glm::vec3 color, bool fill)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	float u = 0.0;

	for (int i = 0; i < 200; i++) {
		vertices.push_back(VertexFormat(center + glm::vec3(R / 1.5 * cos(u), R * sin(u), 0), color));
		indices.push_back(i);
		u = u + 6.28 / 200;
	}
	Mesh* balloon = new Mesh(name);

	if (!fill) {
		balloon->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		balloon->SetDrawMode(GL_TRIANGLE_FAN);
	}
	
	balloon->InitFromData(vertices, indices);
	return balloon;
}

Mesh* Object2D::CreateBalloonTriangle(std::string name, glm::vec3 center, float R, glm::vec3 color, bool fill)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices = { 0, 1, 2 };

	vertices.push_back(VertexFormat(center + glm::vec3(0, -R, 0), color));	// 0
	vertices.push_back(VertexFormat(center + glm::vec3(-10, -R - 20, 0), color));	// 1
	vertices.push_back(VertexFormat(center + glm::vec3(10, -R - 20, 0), color));	// 2

	Mesh* balloonTraingle = new Mesh(name);

	if (!fill) {
		balloonTraingle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		indices.push_back(0);
	}

	balloonTraingle->InitFromData(vertices, indices);
	return balloonTraingle;
}

Mesh* Object2D::CreateLine(std::string name, glm::vec3 center, float R, glm::vec3 color)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices = { 0, 1, 2, 3, 4 };

	vertices.push_back(VertexFormat(center + glm::vec3(0, -R - 20, 0), color));
	vertices.push_back(VertexFormat(center + glm::vec3(-10, -R - 30, 0), color));
	vertices.push_back(VertexFormat(center + glm::vec3(0, -R - 40, 0), color));
	vertices.push_back(VertexFormat(center + glm::vec3(-10, -R - 50, 0), color));
	vertices.push_back(VertexFormat(center + glm::vec3(0, -R - 40, 0), color));

	Mesh* line = new Mesh(name);
	line->SetDrawMode(GL_LINE_STRIP);
	line->InitFromData(vertices, indices);
	return line;

}


Mesh* Object2D::CreateShuriken(std::string name, glm::vec3 center, float length, glm::vec3 color, bool fill)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices = {0, 1, 2, 0, 3, 4, 0, 5, 6, 0, 7, 8};

	vertices.push_back(VertexFormat(center, color));		// 0
	vertices.push_back(VertexFormat(center + glm::vec3(length, 0, 0), color));		// 1
	vertices.push_back(VertexFormat(center + glm::vec3(length / 2, length / 2, 0), color));		// 2
	vertices.push_back(VertexFormat(center + glm::vec3(0, length, 0), color));		// 3
	vertices.push_back(VertexFormat(center + glm::vec3(-length / 2, length / 2, 0), color));		// 4
	vertices.push_back(VertexFormat(center + glm::vec3(-length, 0, 0), color));		// 5
	vertices.push_back(VertexFormat(center + glm::vec3(-length / 2, -length / 2, 0), color));		// 6
	vertices.push_back(VertexFormat(center + glm::vec3(0, -length, 0), color));		// 7 
	vertices.push_back(VertexFormat(center + glm::vec3(length / 2, -length / 2, 0), color));		// 8

	Mesh* shuriken = new Mesh(name);
	if (!fill) {
		shuriken->SetDrawMode(GL_LINE_LOOP);
	}
	
	shuriken->InitFromData(vertices, indices);
	return shuriken;
}

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateLife(std::string name, glm::vec3 center, float R, glm::vec3 color, bool fill)
{
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	float u = 0.0;

	for (int i = 0; i < 200; i++) {
		vertices.push_back(VertexFormat(center + glm::vec3(R * cos(u), R * sin(u), 0), color));
		indices.push_back(i);
		u = u + 6.28 / 200;
	}
	Mesh* life = new Mesh(name);

	if (!fill) {
		life->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		life->SetDrawMode(GL_TRIANGLE_FAN);
	}

	life->InitFromData(vertices, indices);
	return life;
}