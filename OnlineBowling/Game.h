#pragma once
#include "Ball.h"
#include "Floor.h"
#include "Shader.h"
#include "ResourcesManager.h"

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
	}

	void setFloorLength(const float& length)
	{
		floor.size = glm::vec2(1.0f, length);
	}

	void Update(float dt);

	void Render();

private:
	Ball ball;
	Ball pins[10];

	Floor floor;

	Shader shader;
};

