#include "Laborator1.h"
#include <ctime>

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

Laborator1::Laborator1()
{
}

Laborator1::~Laborator1()
{
}

void Laborator1::Init()
{
	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("archer");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Characters/Archer", "Archer.fbx");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("teapot");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
}

void Laborator1::FrameStart()
{

}

void Laborator1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->props.resolution;

	// sets the clear color for the color buffer

	if (keypressed == true) {
		glClearColor(color.x, color.y, color.z, 1);
		// keypressed = false;
	}

	// clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	// render the object
	RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

	// render the object again but with different properties
	RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));

	RenderMesh(meshes["archer"], glm::vec3(archerPos.x, archerPos.y, archerPos.z), glm::vec3(0.01f));

	if (timesPressed % 3 == 0) {
		RenderMesh(meshes["archer"], glm::vec3(-3, 0, 0), glm::vec3(0.01f));
	}
	else if (timesPressed % 3 == 1) {
		RenderMesh(meshes["teapot"], glm::vec3(-3, 0, 0));
	}
	else if (timesPressed % 3 == 2){
		RenderMesh(meshes["sphere"], glm::vec3(-3, 0, 0));
	}

}

void Laborator1::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator1::OnInputUpdate(float deltaTime, int mods)
{
	// treat continuous update based on input
	if (window->KeyHold(GLFW_KEY_W)) {
		archerPos.y += deltaTime + 0.1f;
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		archerPos.y -= deltaTime + 0.1f;
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		archerPos.x -= deltaTime + 0.1f;
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		archerPos.x += deltaTime + 0.1f;
	}

	if (window->KeyHold(GLFW_KEY_Q)) {
		archerPos.z += deltaTime + 0.1f;
	}

	if (window->KeyHold(GLFW_KEY_E)) {
		archerPos.z -= deltaTime + 0.1f;
	}

	if (window->KeyHold(GLFW_KEY_R)) {
		r = sqrt(archerPos.x * archerPos.x + archerPos.y * archerPos.y);
		if (t >= 360) {
			t = 0;
		}
		archerPos.x = r * cos(t);
		archerPos.y = r * sin(t);		
		t = t + deltaTime;
	}
};

void Laborator1::OnKeyPress(int key, int mods)
{
	// add key press event
	
	if (key == GLFW_KEY_F) {
		keypressed = true;
		color.x = ((float)rand() / (RAND_MAX));
		color.y = ((float)rand() / (RAND_MAX));
		color.z = ((float)rand() / (RAND_MAX));
	}

	if (key == GLFW_KEY_C) {
		timesPressed += 1;
	}
};

void Laborator1::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void Laborator1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Laborator1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Laborator1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Laborator1::OnWindowResize(int width, int height)
{
	// treat window resize event
}
