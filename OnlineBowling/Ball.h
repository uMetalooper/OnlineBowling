#pragma once
#include "Sphere.h"

class Ball
{
public:
	Ball()
	{
		position = glm::vec2(0.0f);
		velocity = glm::vec2(0.0f);
		radius = 0.1;
		activeSelf = true;
	}

	void Update(float dt);

	void Draw(Shader& shader)
	{
		renderer.setPosition(glm::vec3(position, radius));
		renderer.setRadius(radius);
		renderer.Draw(shader);
	}

	void setVelocity(glm::vec2 v) { velocity = v; }
	void setPosition(glm::vec2 pos) { position = pos; }
	void setColor(glm::vec3 color) { renderer.setColor(color); }

	void info();

	glm::vec2 getPosition() { return position; }
	glm::vec2 getVelocity() { return velocity; }
	
	void SetActive(bool s) { activeSelf = s; }
	bool GetActive() { return activeSelf; }

	bool IsStop() { return glm::length(velocity) < 0.000001f; }

private:
	bool activeSelf;
	Sphere renderer;

	glm::vec2 position;
	glm::vec2 velocity;
	float radius;
	float mass;
};

