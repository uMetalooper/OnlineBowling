#pragma once
#include "Ball.h"
#include "Floor.h"
#include "Shader.h"
#include "ResourcesManager.h"
#include "Camera.h"

constexpr float ALLEY_WIDTH = 1.0f; // 1 meter wide
constexpr float ALLEY_LENGTH = 18.288f; // 18.288 meters long

constexpr float BALL_RADIUS = 0.1;

class Game
{
public:
	Game();

	void applyImpulse(glm::vec2 v)
	{
		ball.setVelocity(v);
		ballThrown = true;
	}

	unsigned int getPlayerIndex() { return playerIndex; }

	void Update(float dt);

	void Render();

private:
	Camera mainCamera;

	Ball ball;
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

	bool CheckBound(Ball b)
	{
		glm::vec2 ballPos = b.getPosition();
		return ballPos.x > ALLEY_WIDTH / 2.0f || ballPos.x < -ALLEY_WIDTH / 2.0f 
			|| ballPos.y > ALLEY_LENGTH;
	}

	void setCamera(glm::mat4 view)
	{
		shader.setMat4("view", view);
	}

	// gameplay
	unsigned int playerIndex;
	float timer;
};

