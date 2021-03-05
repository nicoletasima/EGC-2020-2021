#include "Laborator4.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"

using namespace std;

Laborator4::Laborator4()
{
}

Laborator4::~Laborator4()
{
}

void Laborator4::Init()
{
	polygonMode = GL_FILL;

	Mesh* mesh = new Mesh("box");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[mesh->GetMeshID()] = mesh;

	// initialize tx, ty and tz (the translation steps)
	translateX = 0;
	translateY = 0;
	translateZ = 0;

	// initialize sx, sy and sz (the scale factors)
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;
	
	// initialize angularSteps
	angularStepOX = 0;
	angularStepOY = 0;
	angularStepOZ = 0;

	// Sun
	translateXSun = 0;
	translateYSun = 0;
	translateZSun = 0;

	scaleXSun = 1;
	scaleYSun = 1;
	scaleZSun = 1;

	// Earth
	angularStepOy_earth = 0;
	angularStep_sun = 0;
	angularStep_moon = 0;
}

void Laborator4::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator4::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-2.5f, 0.5f,-1.5f);
	modelMatrix *= Transform3D::Translate(translateX, translateY, translateZ);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(0.0f, 0.5f, -1.5f);
	modelMatrix *= Transform3D::Scale(scaleX, scaleY, scaleZ);
	RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(2.5f, 0.5f, -1.5f);
	modelMatrix *= Transform3D::RotateOX(angularStepOX);
	modelMatrix *= Transform3D::RotateOY(angularStepOY);
	modelMatrix *= Transform3D::RotateOZ(angularStepOZ);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	// Soare
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(8.0f, 0.5f, -1.5f);
	modelMatrix *= Transform3D::Translate(translateXSun, translateYSun, translateZSun);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	// Pamant
	angularStep_sun += deltaTimeSeconds * 0.75;
	angularStepOy_earth += deltaTimeSeconds * 1.25f;
	angularStep_moon += deltaTimeSeconds * 1.5f;

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(8.0f, 0.5f, -1.5f);
	modelMatrix *= Transform3D::Translate(translateXSun, translateYSun, translateZSun);
	modelMatrix *= Transform3D::RotateOY(angularStep_sun);
	modelMatrix *= Transform3D::Translate(0.0f, 0.0f, 2);
	modelMatrix *= Transform3D::RotateOY(angularStepOy_earth);
	modelMatrix *= Transform3D::Scale(scaleXSun / 2, scaleYSun / 2, scaleZSun / 2);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

	// Luna
	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(8.0f, 0.5f, -1.5f);
	modelMatrix *= Transform3D::Translate(translateXSun, translateYSun, translateZSun);
	modelMatrix *= Transform3D::RotateOY(angularStep_sun);

	modelMatrix *= Transform3D::Translate(0.0f, 0.0f, 2);
	modelMatrix *= Transform3D::RotateOY(angularStep_moon);
	modelMatrix *= Transform3D::Translate(0, 0, 1);
	
	modelMatrix *= Transform3D::RotateOY(angularStepOy_earth);
	modelMatrix *= Transform3D::Scale(scaleXSun / 4, scaleYSun / 4, scaleZSun / 4);
	RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

}

void Laborator4::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator4::OnInputUpdate(float deltaTime, int mods)
{
	// PRIMUL CUB	
	if (window->KeyHold(GLFW_KEY_W)) {
		translateY += deltaTime * 1.5f;
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		translateX -= deltaTime * 1.5f;
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		translateY -= deltaTime * 1.5f;
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		translateX += deltaTime * 1.5f;
	}

	if (window->KeyHold(GLFW_KEY_R)) {
		translateZ += deltaTime * 1.5f;
	}

	if (window->KeyHold(GLFW_KEY_F)) {
		translateZ -= deltaTime * 1.5f;
	}

	// AL DOILEA CUB
	if (window->KeyHold(GLFW_KEY_1)) {
		scaleX += deltaTime * 1.5f;
		scaleY += deltaTime * 1.5f;
		scaleZ += deltaTime * 1.5f;
	}

	if (window->KeyHold(GLFW_KEY_2)) {
		scaleX -= deltaTime * 1.5f;
		scaleY -= deltaTime * 1.5f;
		scaleZ -= deltaTime * 1.5f;
	}

	if (window->KeyHold(GLFW_KEY_3)) {
		angularStepOX += deltaTime * 1.5f;
	}
	if (window->KeyHold(GLFW_KEY_4)) {
		angularStepOX -= deltaTime * 1.5f;
	}

	if (window->KeyHold(GLFW_KEY_5)) {
		angularStepOY += deltaTime * 1.5f;
	}

	if (window->KeyHold(GLFW_KEY_6)) {
		angularStepOY -= deltaTime * 1.5f;
	}

	if (window->KeyHold(GLFW_KEY_7)) {
		angularStepOZ += deltaTime * 1.5f;
	}

	if (window->KeyHold(GLFW_KEY_8)) {
		angularStepOZ -= deltaTime * 1.5f;
	}

	// miscare Soare
	if (window->KeyHold(GLFW_KEY_UP)) {
		translateYSun += deltaTime * 1.5f;
	}

	if (window->KeyHold(GLFW_KEY_DOWN)) {
		translateYSun -= deltaTime * 1.5f;
	}

	if (window->KeyHold(GLFW_KEY_LEFT)) {
		translateXSun -= deltaTime * 1.5f;
	}

	if (window->KeyHold(GLFW_KEY_RIGHT)) {
		translateXSun += deltaTime * 1.5f;
	}

	if (window->KeyHold(GLFW_KEY_M)) {
		translateZSun += deltaTime * 1.5f;
	}

	if (window->KeyHold(GLFW_KEY_N)) {
		translateZSun -= deltaTime * 1.5f;
	}

}

void Laborator4::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_SPACE)
	{
		switch (polygonMode)
		{
		case GL_POINT:
			polygonMode = GL_FILL;
			break;
		case GL_LINE:
			polygonMode = GL_POINT;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}
}

void Laborator4::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator4::OnWindowResize(int width, int height)
{
}
