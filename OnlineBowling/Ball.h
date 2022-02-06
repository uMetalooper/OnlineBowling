#pragma once
#include "Sphere.h"
#include "glm_vec_helper.h"

class Ball
{
public:
	Ball()
	{
		position = glm::vec2(0.0f);
		velocity = glm::vec2(0.0f);
		radius = 0.05;
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
	
private:
	Sphere renderer;

	glm::vec2 position;
	glm::vec2 velocity;
	float radius;
	float mass;
};

