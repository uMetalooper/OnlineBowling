#pragma once
#include "Ball.h"
#include "Floor.h"
#include "Shader.h"
#include "ResourcesManager.h"

constexpr float ALLEY_WIDTH = 1.0f; // 1 meter wide
constexpr float ALLEY_LENGTH = 18.288f; // 18.288 meters long

constexpr float BALL_RADIUS = 0.1;

class Game
{
public:
	Game();

	void setCamera(glm::mat4 view)
	{
		shader.setMat4("view", view);
	}

	void applyImpulse(glm::vec2 v)
	{
		ball.setVelocity(v);
		ballThrown = true;
	}

	void setFloorLength(const float& length)
	{
		floor.size = glm::vec2(1.0f, length);
	}

	void Update(float dt);

	void Render();

	Ball ball;
private:
	bool ballThrown;
	Ball pins[10];
	bool touchedIndex[10];
	int removedIndex;

	Floor floor;

	Shader shader;

	void ResetBall();
	void ResetPins();

	bool CheckCollisions(Ball& one, Ball& two);
	void ApplyCollision(Ball& one, Ball& two);
};

