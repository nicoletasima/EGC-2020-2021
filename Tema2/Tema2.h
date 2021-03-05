#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include <vector>

#include "Transform3D.h"
#include "LabCamera.h"

#define PLATFORMS_NR 27

struct Platform {
	glm::vec3 color;
	glm::vec3 originalColor;
	glm::vec3 position;
	bool wasCollision;
	float zLength;
};

struct Player {
	glm::vec3 pos;	// position
	bool jump_up;				// if jump
	bool jump_down;
	float radius;		// raza
	float speed;		// viteza
};

class Tema2 : public SimpleScene
{
public:
	Tema2();
	~Tema2();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);

	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1));

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
	bool isCollision(glm::vec3 sphereCenter, glm::vec3 platformCenter, float length);
	// Create FuelBar
	Mesh* CreatePowerBar(std::string, glm::vec3 leftBottomCorner, float width, float powerLength, glm::vec3 color, bool fill);
	bool isPointInsideAABB(glm::vec3 point, glm::vec3 center, float length);

	void RenderSimpleMeshTexture(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL);

protected:
	// Platforms
	float platformSpeed;
	float gap;

	bool orangecoll;
	int time;
	Platform platforms[PLATFORMS_NR];
	int randColor;
	// 0 - blue, 1 - red, 2 - green, 3 - purple, 4 - yellow, 5 - orange
	glm::vec3 colors[27] = {glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1), glm::vec3(1, 1, 0), 
							glm::vec3(1, 0.6f, 0), glm::vec3(0, 0, 1) , glm::vec3(0, 0, 1) , glm::vec3(0, 0, 1) , glm::vec3(0, 0, 1) , glm::vec3(1, 0, 0), 
							glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1) , glm::vec3(0, 1, 0), glm::vec3(0, 0, 1) , glm::vec3(0, 0, 1),
							glm::vec3(1, 0.6f, 0), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(1, 1, 0),
							glm::vec3(0, 0, 1) , glm::vec3(0, 0, 1) , glm::vec3(0, 0, 1)};
	int randomColor;

	float const grav = 0.1;
	Player player;

	// Fuel
	glm::vec3 backFuel;
	glm::vec3 frontFuel;
	float fuelLeft;

	int lives;

	float newSpeed;

	// Camera
	bool renderCamera;
	Skyroads::Camera* camera;
	glm::mat4 projectionMatrix;
	float off;

	glm::mat4 playerM;
	bool playerCollision;

	// textures
	std::unordered_map<std::string, Texture2D*> mapTextures;
};
