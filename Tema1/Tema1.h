#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

struct Shuriken
{
	std::string name;
	glm::vec3 center;
	float length;
	glm::vec3 color;
	bool fill;
};

struct Balloon
{
	std::string name;
	glm::vec3 center;
	float R;
	glm::vec3 color;
	bool fill;
};


class Tema1 :public SimpleScene
{
	public:
		Tema1();
		~Tema1();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;
	
	protected:
		glm::mat3 modelMatrix;
		
		// balloons
		float R = 50.0;
		glm::vec3 triangleStart;
		float triangleLength;
		glm::vec3 color, yellow = glm::vec3(1, 1, 0), red = glm::vec3(1, 0, 0);
		std::vector<int> balloons;
		float tyBalloon;
		float balloonPosX[8], balloonPosY[8];	// retine pozitiile curente ale fiecarui balon in miscare	-> coliziuni
		float randomPosX[8];
		float scaleFactor[8];

		// arrow
		float lengthArrowHead = 20;
		glm::vec3 startArrowTail;
		float lengthArrowTail;
		float txArrow, tyArrow;
		bool buttonRelease;
		bool mouseControlArrow;
		float angularStepArrow;
		bool keyControlArrow;
		float posNowY;
		float arrowSpeed;
		float currentPosX, currentPosY;	// pozitia curenta a sagetii -> coliziuni

		// Bow
		float tyBow;
		glm::vec3 origin;
		float angularStepBow;
		bool mouseControlBow;
		float currBowPosY;		// posizita curenta a arcului -> coliziuni

		// Shuriken
		float angularStepShuriken1, angularStepShuriken2;
		float txShuriken;
		float currShurikenPosX[5], currShurikenPosY[5]; // pozitiile curente -> coliziuni shuriken

		// power bar
		float powerLength;

		// Lives
		int lives;

		// Score;
		int score;

		// Collisions
		bool collisionBowShu[5] = { false, false, false, false, false };
		bool collisionArrowShu[5] = { false, false, false, false, false };
		bool collisionArrowBalloon[5] = { false, false, false, false, false };
};