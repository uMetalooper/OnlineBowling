#include "Ball.h"
#include <glm/gtx/norm.hpp>

#include <iostream>

constexpr float fricCoeff = 0.03f;
constexpr float g = 9.8f;

void Ball::Update(float dt)
{
	if (glm::length(velocity) <= 0.000001f) return;
	// apply friction
	glm::vec2 acceleration = -glm::normalize(velocity);
	acceleration = acceleration * fricCoeff * g;
	glm::vec2 velocityChange = acceleration * dt;
	if (glm::length(velocityChange) > glm::length(velocity)) velocity = glm::vec2(0.0f);
	else velocity += velocityChange;

	// update position
	position += velocity * dt;
}

void Ball::info()
{
	std::cout << "Position: x = " << position.x << ", y = " << position.y << std::endl;
	std::cout << "Velocity: x = " << velocity.x << ", y = " << velocity.y << std::endl;
}
