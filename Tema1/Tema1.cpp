#include <iostream>
#include <ctime>
#include <vector>

#include "Tema1.h"
#include <Laboratoare\Tema1\Object2D.h>
#include <Laboratoare\Tema1\Transform2D.h>

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);
	
	glm::vec3 center = glm::vec3(0, 0, 0);

	// Arrow
	startArrowTail = glm::vec3(0, 0, 0);
	lengthArrowTail = 70.0f;
	txArrow = 0.0f;
	tyArrow = 0.0f;
	buttonRelease = false;
	mouseControlArrow = true;
	keyControlArrow = true;
	triangleStart = glm::vec3(0, 0, 0);
	triangleLength = 20.0f;
	posNowY = 0.0f;
	arrowSpeed = 500.0f;
	currentPosX = 20.0f;
	currentPosY = 400.0f;
	angularStepArrow = 0.0f;

	// Bow Moves
	tyBow = 0;
	currBowPosY = 400;
	origin = glm::vec3(0, 0, 0);
	angularStepBow = 0.0f;
	mouseControlBow = true;

	// Shuriken
	angularStepShuriken1 = 0.0f;
	angularStepShuriken2 = 0.0f;
	txShuriken = 0.0f;
	for (int i = 0; i < 5; i++) {
		currShurikenPosY[i] = 100.0f + std::rand() % 600;
		currShurikenPosX[i] = 1320 + std::rand() % 380;
	}

	// Balloons
	tyBalloon = 0.0f;
	for (int i = 0; i < 8; i++) {
		randomPosX[i] = 200.0f + std::rand() % 1080;
		balloonPosY[i] = -(50 + rand() % 500);
		scaleFactor[i] = 1;
	}
	
	// Power bar
	powerLength = 0.0f;

	// Lives
	lives = 3;

	// Score
	score = 0;

	Mesh* bow = Object2D::CreateBow("bow", origin, glm::vec3(1, 0, 0));
	AddMeshToList(bow);

	Mesh* arrowHead = Object2D::CreateArrowHead("arrowHead", startArrowTail, lengthArrowTail, glm::vec3(0, 0, 1), true);
	AddMeshToList(arrowHead);

	Mesh* arrowTail = Object2D::CreateArrowTail("arrowTail", startArrowTail, lengthArrowTail, glm::vec3(0, 0, 1));
	AddMeshToList(arrowTail);

	Mesh* red_balloon = Object2D::CreateBalloon("red balloon", center, R, red, true);
	AddMeshToList(red_balloon);

	Mesh* yellow_balloon = Object2D::CreateBalloon("yellow balloon", center, R, yellow, true);
	AddMeshToList(yellow_balloon);

	Mesh* red_line = Object2D::CreateLine("red line", center, R, red);
	AddMeshToList(red_line);

	Mesh* yellow_line = Object2D::CreateLine("yellow line", center, R, yellow);
	AddMeshToList(yellow_line);

	Mesh* red_balloonTriangle = Object2D::CreateBalloonTriangle("red balloonTriangle", center, R, red, true);
	AddMeshToList(red_balloonTriangle);

	Mesh* yellow_balloonTriangle = Object2D::CreateBalloonTriangle("yellow balloonTriangle", center, R, yellow, true);
	AddMeshToList(yellow_balloonTriangle);

	Mesh* shuriken = Object2D::CreateShuriken("shuriken", glm::vec3(0, 0, 0), 40, glm::vec3(1, 0, 1), true);
	AddMeshToList(shuriken);

	Mesh* power_bar = Object2D::CreatePowerBar("power bar", glm::vec3(0, 0, 0), 5, 10, glm::vec3(1, 1, 1), true);
	AddMeshToList(power_bar);

	Mesh* life = Object2D::CreateLife("life", glm::vec3(0, 0, 0), 10, glm::vec3(1, 0, 0), true);
	AddMeshToList(life);
}

void Tema1::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->GetResolution();
		// Bow	
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(30, currBowPosY);
		modelMatrix *= Transform2D::Rotate(angularStepBow);
		RenderMesh2D(meshes["bow"], shaders["VertexColor"], modelMatrix);
		if (mouseControlArrow == true) {
			RenderMesh2D(meshes["arrowTail"], shaders["VertexColor"], modelMatrix);
			RenderMesh2D(meshes["arrowHead"], shaders["VertexColor"], modelMatrix);
		}

		// Arrow 
		modelMatrix = glm::mat3(1);
		if (mouseControlArrow == false && keyControlArrow == false) {
			txArrow = deltaTimeSeconds * arrowSpeed * cos(angularStepArrow);
			tyArrow = deltaTimeSeconds * arrowSpeed * sin(angularStepArrow);
			currentPosX += txArrow;			// pozitia curenta pe X
			currentPosY += tyArrow;			// pozitia curenta pe y
			modelMatrix *= Transform2D::Translate(currentPosX, currentPosY);
			modelMatrix *= Transform2D::Rotate(angularStepArrow);
			RenderMesh2D(meshes["arrowTail"], shaders["VertexColor"], modelMatrix);
			RenderMesh2D(meshes["arrowHead"], shaders["VertexColor"], modelMatrix);

		}

		if ( currentPosX > 1280 || currentPosY > 720 || currentPosY < 0) {
			arrowSpeed = 0;
			mouseControlArrow = true;
			keyControlArrow = true;
			powerLength = 0;
			currentPosX = 20.0f;
			currentPosY = currBowPosY;
			collisionArrowShu[1] = false;
			collisionArrowShu[0] = false;
		}


		// Balloon
		tyBalloon = deltaTimeSeconds * 100;

		for (int i = 0; i < 3; i++) {
			modelMatrix = glm::mat3(1);
			balloonPosY[i] += tyBalloon;
			modelMatrix *= Transform2D::Translate(randomPosX[i], balloonPosY[i]);
			RenderMesh2D(meshes["yellow balloon"], shaders["VertexColor"], modelMatrix);
			RenderMesh2D(meshes["yellow line"], shaders["VertexColor"], modelMatrix);
			RenderMesh2D(meshes["yellow balloonTriangle"], shaders["VertexColor"], modelMatrix);
			if (balloonPosY[i] - R - 50 >= 720 || scaleFactor[i] <= 0) {
				collisionArrowBalloon[i] = false;
				randomPosX[i] = 200 + std::rand() % 1080;
				balloonPosY[i] = -(50 + rand() % 500);
				scaleFactor[i] = 1;
			}

			if (pow(currentPosX + 90.0 * cos(angularStepArrow) - randomPosX[i], 2) / pow(R / 1.5, 2) +
				pow(currentPosY + 90.0 * sin(angularStepArrow) - balloonPosY[i], 2) / pow(R, 2) <= 1 &&
				collisionArrowBalloon[i] == false) {
				score -= 7;
				collisionArrowBalloon[i] = true;
				cout << "Score: " << score << endl;
				while (scaleFactor[i] >= 0) {
					modelMatrix *= Transform2D::Scale(scaleFactor[i], scaleFactor[i]);
					scaleFactor[i] -= deltaTimeSeconds * 10;
					RenderMesh2D(meshes["yellow balloon"], shaders["VertexColor"], modelMatrix);
					RenderMesh2D(meshes["yellow line"], shaders["VertexColor"], modelMatrix);
					RenderMesh2D(meshes["yellow balloonTriangle"], shaders["VertexColor"], modelMatrix);
				}
			}
		}

		for (int i = 3; i < 7; i++) {
			modelMatrix = glm::mat3(1);
			balloonPosY[i] += tyBalloon;
			modelMatrix *= Transform2D::Translate(randomPosX[i], balloonPosY[i]);
			RenderMesh2D(meshes["red balloon"], shaders["VertexColor"], modelMatrix);
			RenderMesh2D(meshes["red line"], shaders["VertexColor"], modelMatrix);
			RenderMesh2D(meshes["red balloonTriangle"], shaders["VertexColor"], modelMatrix);
			if (balloonPosY[i] - R - 50 >= 720 || scaleFactor[i] <= 0) {
				collisionArrowBalloon[i] = false;
				randomPosX[i] = 200 + std::rand() % 1080;
				balloonPosY[i] = -(50 + rand() % 500);
				scaleFactor[i] = 1;
			}

			if (pow(currentPosX + 90.0 * cos(angularStepArrow) - randomPosX[i], 2) / pow(R / 1.5, 2) +
				pow(currentPosY + 90.0 * sin(angularStepArrow) - balloonPosY[i], 2) / pow(R, 2) <= 1 && 
				collisionArrowBalloon[i] == false) {
				score += 10;
				collisionArrowBalloon[i] = true;
				cout << "Score: " << score << endl;
				while (scaleFactor[i] >= 0) {
					modelMatrix *= Transform2D::Scale(scaleFactor[i], scaleFactor[i]);
					scaleFactor[i] -= deltaTimeSeconds * 10;
					RenderMesh2D(meshes["red balloon"], shaders["VertexColor"], modelMatrix);
					RenderMesh2D(meshes["red line"], shaders["VertexColor"], modelMatrix);
					RenderMesh2D(meshes["red balloonTriangle"], shaders["VertexColor"], modelMatrix);
				}
			}
		}


		// Shuriken
		txShuriken = deltaTimeSeconds * 100;
		angularStepShuriken1 += deltaTimeSeconds * 5;
		angularStepShuriken2 += deltaTimeSeconds * 10;

		for (int i = 0; i < 5; i++) {
			modelMatrix = glm::mat3(1);
			currShurikenPosX[i] -= txShuriken;
			modelMatrix *= Transform2D::Translate(currShurikenPosX[i], currShurikenPosY[i]);
			if (i == 1 || i == 3) {
				modelMatrix *= Transform2D::Rotate(angularStepShuriken2);
			}
			else {
				modelMatrix *= Transform2D::Rotate(angularStepShuriken1);
			}
			RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);
			if (currShurikenPosX[i] + 40 < 0 || collisionArrowShu[i] == true) {
				currShurikenPosY[i] = 100.0f + std::rand() % 600;
				currShurikenPosX[i] = 1320 + std::rand() % 380;
				collisionBowShu[i] = false;
				collisionArrowShu[i] = false;
			}
		}

		for (int i = 0; i < 5; i++) {
			if (sqrt(pow(currentPosX + 90.0f * cos(angularStepArrow) - currShurikenPosX[i], 2)
				+ pow(currentPosY + 90.0f * sin(angularStepArrow) - currShurikenPosY[i], 2)) <= 40.0f 
				&& collisionArrowShu[i] == false && mouseControlArrow == false) {
				collisionArrowShu[i] = true;
				score += 5;
				cout << "Score: " << score << endl;
			}
		}

		for (int i = 0; i < 5; i++) {
			float dx = 30 - currShurikenPosX[i];
			float dy = currBowPosY - currShurikenPosY[i];
			float distance = sqrt(dx * dx + dy * dy);
			if (distance < 90 && collisionBowShu[i] == false) {
				lives--;
				collisionBowShu[i] = true;
				if (lives == 0) {
					exit(0);
				}

			}
		}


		// Power Bar
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(0, 20);
		modelMatrix *= Transform2D::Translate(5.0f, 2.5f);
		modelMatrix *= Transform2D::Scale(powerLength, 1);
		modelMatrix *= Transform2D::Translate(-5.0f, 2.5f);
		RenderMesh2D(meshes["power bar"], shaders["VertexColor"], modelMatrix);

		// Lives
		if (lives >= 1) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(10, 705);
			RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
		}

		if (lives >= 2) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(35, 705);
			RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
		}

		if (lives == 3) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= Transform2D::Translate(60, 705);
			RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
		}
}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W)) {
		if (currBowPosY + R <= 720) {
			currBowPosY += deltaTime * 200;
		}
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		if (currBowPosY - R >= 0) {
			currBowPosY -= deltaTime * 200;
		}
	}

	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && mouseControlArrow == true) {
		if (arrowSpeed <= 4000) {
			arrowSpeed += 10;
			powerLength += 0.5;
		}
	}
}

void Tema1::OnKeyPress(int key, int mods)
{

}

void Tema1::OnKeyRelease(int key, int mods)
{

}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	int mouseYnow = window->GetResolution().y - mouseY;
	if (mouseControlBow == true) {
			angularStepBow = atan((mouseYnow - currBowPosY) / mouseX);
	}

}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	/*if (mouseControlArrow == false) {

	}*/
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
		mouseControlArrow = false;
		keyControlArrow = false;
		angularStepArrow = angularStepBow;
		currentPosX = 20.0f;
		currentPosY = currBowPosY;
	}
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{

}

void Tema1::OnWindowResize(int width, int height)
{

}

