#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include <Laboratoare/Tema1/Transform2D.h>

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{

	gap = 1.2f;
	platformSpeed = 0.f;
	off = 0.0;
	playerCollision = false;

	player.pos = glm::vec3(0, 0.1f, 0);
	player.jump_up = false;
	player.jump_down = false;
	player.speed = 0;
	player.radius = 0.1f;

	backFuel = glm::vec3(-0.75f, 1.f, 0.f);
	frontFuel = glm::vec3(-0.75f, 1.f, 0.01f);
	fuelLeft = 1.f;

	orangecoll = false;
	time = 0;

	lives = 3;
	


	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = Tema2::CreatePowerBar("back", glm::vec3(0, 0, 0), 0.15, 2, glm::vec3(1, 1, 1), true);
		meshes[mesh->GetMeshID()] = mesh;

		mesh = Tema2::CreatePowerBar("front", glm::vec3(0, 0, 0), 0.15, 2, glm::vec3(0, 1, 0), true);
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("heart");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "heart.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Platforms 
	{	
		int x = -0.8;
		for (int i = 0; i < 3; i++) {
			if (i == 0) {
				platforms[i].position = glm::vec3(-0.8, 0, -1);
				randColor = rand() % 27;
				platforms[i].color = colors[randColor];
				platforms[i].originalColor = platforms[i].color;
				platforms[i].wasCollision = false;
				platforms[i].zLength = 3;
			}

			if (i == 1) {
				platforms[i].position = glm::vec3(0, 0, -1);
				platforms[i].color = glm::vec3(0, 0, 1);
				platforms[i].originalColor = platforms[i].color;
				platforms[i].wasCollision = false;
				platforms[i].zLength = 7;
			}

			if (i == 2) {
				platforms[i].position = glm::vec3(0.8, 0, -1);
				randColor = rand() % 27;
				platforms[i].color = colors[randColor];
				platforms[i].originalColor = platforms[i].color;
				platforms[i].wasCollision = false;
				platforms[i].zLength = 3;
			}
			
		}
		for (int i = 3; i < PLATFORMS_NR; i++) {
			if (i % 3 == 0) {
				platforms[i].position = glm::vec3(-0.8, 0, platforms[i - 3].position.z - platforms[i - 3].zLength / 2 - gap);
				randColor = rand() % 5;
				platforms[i].color = colors[randColor];
				platforms[i].originalColor = platforms[i].color;
				platforms[i].wasCollision = false;
				platforms[i].zLength = 3 + rand() % 5;
			}

			if (i % 3 == 1) {
				platforms[i].position = glm::vec3(0, 0, platforms[i - 3].position.z - platforms[i - 3].zLength / 2 - gap);
				randColor = rand() % 5;
				platforms[i].color = colors[randColor];
				platforms[i].originalColor = platforms[i].color;
				platforms[i].wasCollision = false;
				platforms[i].zLength = 3 + rand() % 5;
			}

			if (i % 3 == 2) {
				platforms[i].position = glm::vec3(0.8, 0, platforms[i - 3].position.z - platforms[i - 3].zLength / 2 - gap);
				randColor = rand() % 5;
				platforms[i].color = colors[randColor];
				platforms[i].originalColor = platforms[i].color;
				platforms[i].wasCollision = false;
				platforms[i].zLength = 3 + rand() % 5;
			}
			
		}
	 }

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("ShaderTema2");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("SphereShader");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/SphereVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/SphereFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	// Camera
	renderCamera = false;	// thirdPerson = false, otherwise = firstPerson = true
	camera = new Skyroads::Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	// TEMA 3 -> TEXTURES
	const string textureLoc = "Source/Laboratoare/Tema2/Textures/";

	// Load textures

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "crate.jpg").c_str(), GL_REPEAT);
		mapTextures["crate"] = texture;
	}

	// Create quad
	{
		vector<glm::vec3> vertices
		{
			glm::vec3(0.5f,   0.5f, 0.0f),	// Top Right
			glm::vec3(0.5f,  -0.5f, 0.0f),	// Bottom Right
			glm::vec3(-0.5f, -0.5f, 0.0f),	// Bottom Left
			glm::vec3(-0.5f,  0.5f, 0.0f),	// Top Left
		};

		vector<glm::vec3> normals
		{
			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0)
		};

		// TODO : Complete texture coordinates for the square
		vector<glm::vec2> textureCoords
		{
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f)
		};

		vector<unsigned short> indices =
		{
			0, 1, 3,
			1, 2, 3
		};

		Mesh* mesh = new Mesh("square");
		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Shaders
	{
		Shader* shader = new Shader("TextureShader");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/BoxVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/BoxFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// TODO: Crete the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

	// set vertex position attribute
	glEnableVertexAttribArray(0);	// pipe-ul pe care se trimit date 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	meshes[name]->vertices = vertices;
	meshes[name]->indices = indices;
	return meshes[name];
}


void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	
	if (renderCamera) {
		
		camera->Set(glm::vec3(player.pos.x + off, player.pos.y + 0.1, player.pos.z - player.radius - 0.2), glm::vec3(player.pos.x + off, player.pos.y + 0.1, player.pos.z - player.radius - 0.3), glm::vec3(0, 1, 0));	// first

		}
		
	else {
		camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)); // thid
	}
	// Player
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= glm::translate(modelMatrix, player.pos);
		modelMatrix *= glm::scale(modelMatrix, glm::vec3(0.4f, 0.4f, 0.4f));
		playerM = modelMatrix;
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0));
	}

	{
		playerCollision = false;
		for (int i = 0; i < PLATFORMS_NR; i++) {
			// redesenare cand iese din cadru
			if (platforms[i].position.z > 3) {
				if (i == 0) {
					platforms[i].position = glm::vec3(-0.8, 0, platforms[24].position.z - platforms[24].zLength / 2 - gap);
					randColor = rand() % 27;
					platforms[i].color = colors[randColor];
					platforms[i].originalColor = platforms[i].color;
					platforms[i].wasCollision = false;
					platforms[i].zLength = 3 + rand() % 5;
				}

				if (i == 1) {
					platforms[i].position = glm::vec3(0, 0, platforms[25].position.z - platforms[25].zLength / 2 - gap);
					randColor = rand() % 27;
					platforms[i].color = colors[randColor];
					platforms[i].originalColor = platforms[i].color;
					platforms[i].wasCollision = false;
					platforms[i].zLength = 3 + rand() % 5;
				}

				if (i == 2) {
					platforms[i].position = glm::vec3(0.8, 0, platforms[26].position.z - platforms[26].zLength / 2 - gap);
					randColor = rand() % 27;
					platforms[i].color = colors[randColor];
					platforms[i].originalColor = platforms[i].color;
					platforms[i].wasCollision = false;
					platforms[i].zLength = 3 + rand() % 5;
				}

				if (i % 3 == 0 && i >= 3) {
					platforms[i].position = glm::vec3(-0.8, 0, platforms[i - 3].position.z - platforms[i - 3].zLength / 2 - gap);
					randColor = rand() % 27;
					platforms[i].color = colors[randColor];
					platforms[i].originalColor = platforms[i].color;
					platforms[i].wasCollision = false;
					platforms[i].zLength = 3 + rand() % 5;
				}

				if (i % 3 == 1 && i >= 3) {
					platforms[i].position = glm::vec3(0, 0, platforms[i - 3].position.z - platforms[i - 3].zLength / 2 - gap);
					randColor = rand() % 27;
					platforms[i].color = colors[randColor];
					platforms[i].originalColor = platforms[i].color;
					platforms[i].wasCollision = false;
					platforms[i].zLength = 3 + rand() % 5;
				}

				if (i % 3 == 2 && i >= 3) {
					platforms[i].position = glm::vec3(0.8, 0, platforms[i - 3].position.z - platforms[i - 3].zLength / 2 - gap);
					randColor = rand() % 27;
					platforms[i].color = colors[randColor];
					platforms[i].originalColor = platforms[i].color;
					platforms[i].wasCollision = false;
					platforms[i].zLength = 3 + rand() % 5;
				}
			}

			if (isCollision(player.pos, platforms[i].position, platforms[i].zLength) == true) {
				playerCollision = true;
				platforms[i].color = glm::vec3(1, 0, 1);	// purple

				if (platforms[i].originalColor == glm::vec3(0, 0, 1)) { // blue
					platforms[i].wasCollision = true;
				}

				if (platforms[i].originalColor == glm::vec3(0, 1, 0) && platforms[i].wasCollision == false) { // green
					platforms[i].wasCollision = true;
					if (fuelLeft <= 0.85) {
						fuelLeft += 0.15;
					}
					else {
						fuelLeft = 1;
					}
					RenderSimpleMesh(meshes["sphere"], shaders["SphereShader"], playerM, glm::vec3(1, 0, 0));
				}

				if (platforms[i].originalColor == glm::vec3(1, 0, 0) && platforms[i].wasCollision == false) { // red
					platforms[i].wasCollision = true;
					lives--;
					if (lives == 0) {
						exit(-1);
					}
					RenderSimpleMesh(meshes["sphere"], shaders["SphereShader"], playerM, glm::vec3(1, 0, 0));
				}

				if (platforms[i].originalColor == glm::vec3(1, 1, 0) && platforms[i].wasCollision == false) { // yellow
					platforms[i].wasCollision = true;
					if (fuelLeft >= 0.05) {
						fuelLeft -= 0.05;
					}
					RenderSimpleMesh(meshes["sphere"], shaders["SphereShader"], playerM, glm::vec3(1, 0, 0));
				}

				if (platforms[i].originalColor == glm::vec3(1, 0.6f, 0)) {	// orange
					platforms[i].wasCollision = true;
					orangecoll = true;
					RenderSimpleMesh(meshes["sphere"], shaders["SphereShader"], playerM, glm::vec3(1, 0, 0));
				}
				

			}
			else if (isCollision(player.pos, platforms[i].position, platforms[i].zLength) == false) {
				if (platforms[i].wasCollision == true) {
					platforms[i].color = platforms[i].originalColor;
				}
			}

			if (orangecoll == true) {
				time++;
				if (time >= 10000) {
					time = 0;
					orangecoll = false;
				}
	
				glm::mat4 modelMatrix = glm::mat4(1);
				platforms[i].position.z += deltaTimeSeconds * newSpeed;
				modelMatrix *= glm::translate(modelMatrix, platforms[i].position);
				modelMatrix *= glm::scale(modelMatrix, glm::vec3(1, 0.1, platforms[i].zLength));
				RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, platforms[i].color);
			}
			else if (orangecoll == false) {
				glm::mat4 modelMatrix = glm::mat4(1);
				platforms[i].position.z += deltaTimeSeconds * platformSpeed;
				modelMatrix *= glm::translate(modelMatrix, platforms[i].position);
				modelMatrix *= glm::scale(modelMatrix, glm::vec3(1, 0.1, platforms[i].zLength));
				RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, platforms[i].color);
			}
		}

		if ((playerCollision == false) && (player.jump_up == false)) {
			player.pos.y -= 0.5;
			if (player.pos.y < -5) {
				exit(-1);
			}
		}

	}


	// Fuel
	{
		glm::mat3 modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(-2.3, 2);
		modelMatrix *= Transform2D::Scale(fuelLeft, 1);
		RenderMesh2D(meshes["front"], shaders["VertexColor"], modelMatrix);
		
		if (fuelLeft > 0) {
			fuelLeft -= 0.0003f;
		}

		if (fuelLeft <= 0) {
			exit(-1);
		}
	
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(-2.3, 2);
		RenderMesh2D(meshes["back"], shaders["VertexColor"], modelMatrix);
	}

	// Lives
	{
		if (lives >= 3) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= glm::translate(modelMatrix, glm::vec3(1, 1, 0));
			modelMatrix *= glm::scale(modelMatrix, glm::vec3(0.01, 0.01, 0.01));
			modelMatrix *= Transform3D::RotateOX(-3.14 / 2);
			RenderSimpleMesh(meshes["heart"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0));
		}

		if (lives >= 2) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= glm::translate(modelMatrix, glm::vec3(0.85, 1, 0));
			modelMatrix *= glm::scale(modelMatrix, glm::vec3(0.01, 0.01, 0.01));
			modelMatrix *= Transform3D::RotateOX(-3.14 / 2);
			RenderSimpleMesh(meshes["heart"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0));
		}

		if (lives >= 1) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= glm::translate(modelMatrix, glm::vec3(0.7, 1, 0));
			modelMatrix *= glm::scale(modelMatrix, glm::vec3(0.01, 0.01, 0.01));
			modelMatrix *= Transform3D::RotateOX(-3.14 / 2);
			RenderSimpleMesh(meshes["heart"], shaders["ShaderTema2"], modelMatrix, glm::vec3(1, 0, 0));
		}
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f,  2.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		RenderSimpleMeshTexture(meshes["square"], shaders["Texture"], modelMatrix, mapTextures["crate"]);
	}
}

void Tema2::FrameEnd()
{
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;
	glUseProgram(shader->program);

	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	GLint timeLocation = glGetUniformLocation(shader->GetProgramID(), "time");
	glUniform1f(timeLocation, (GLfloat)Engine::GetElapsedTime());

	GLint locObject = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(locObject, 1, glm::value_ptr(color));

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);

}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	// add key press event
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {

	}

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		if (orangecoll == false) {
			if (window->KeyHold(GLFW_KEY_W)) {
				if (platformSpeed <= 15) {
					platformSpeed += 0.01;
				}
			}

			if (window->KeyHold(GLFW_KEY_S)) {
				if (platformSpeed > 0.01) {
					platformSpeed -= 0.01;
				}
			}
		}
		else if (orangecoll == true) {
			newSpeed = 5;
		}
		
		if (window->KeyHold(GLFW_KEY_A)) {
			player.pos.x -= 0.015;
			off -= 0.015;
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			player.pos.x += 0.015;
			off += 0.015;
		}
			
		if (player.jump_up == true) {
			if (player.speed > -5) {
				player.pos.y += player.speed * deltaTime;
				player.speed -= 0.2;
			}
			else {
				player.pos.y = 0.1;
				player.jump_up = false;
			}
		}
		
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_SPACE && player.jump_up == false) {
		player.jump_up = true;
		player.speed = 5;

	}

	if (key == GLFW_KEY_C) {
		renderCamera = !renderCamera;
	}

}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}

bool Tema2::isCollision(glm::vec3 sphereCenter, glm::vec3 platformCenter, float length)
{
	float x = max(platformCenter.x - 0.25f, min(sphereCenter.x, platformCenter.x + 0.25f));
	float y = max(platformCenter.y - 0.1f, min(sphereCenter.y, platformCenter.y + 0.1f));
	float z = max(platformCenter.z - length / 4, min(sphereCenter.z, platformCenter.z + length / 4));

	// this is the same as isPointInsideSphere
	float distance = sqrt((x - sphereCenter.x) * (x - sphereCenter.x) +
		(y - sphereCenter.y) * (y - sphereCenter.y) +
		(z - sphereCenter.z) * (z - sphereCenter.z));
	return distance < player.radius;
}

Mesh* Tema2::CreatePowerBar(std::string name, glm::vec3 leftBottomCorner, float width, float powerLength, glm::vec3 color, bool fill)
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

void Tema2::RenderSimpleMeshTexture(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	if (texture1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
		glUniform1i(glGetUniformLocation(shader->program, "mixed"), false);
	}

	if (texture2)
	{
		//TODO : activate texture location 1
		glActiveTexture(GL_TEXTURE1);
		//TODO : Bind the texture2 ID
		glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
		//TODO : Send texture uniform value
		glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
		glUniform1i(glGetUniformLocation(shader->program, "mixed"), true);
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}