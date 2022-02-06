#include "Ball.h"
#include "glm_vec_helper.h"
#include <glm/gtx/norm.hpp>

#include <iostream>

constexpr float fricCoeff = 0.03f;
constexpr float g = 9.8f;

void Ball::Update(float dt)
{
	if (Vec2::Magnitude(velocity) <= 0.000001f) return;
	// apply friction
	glm::vec2 acceleration = -Vec2::Normalize(velocity);
	acceleration = acceleration * fricCoeff * g;
	glm::vec2 velocityChange = acceleration * dt;
	if (Vec2::Magnitude(velocityChange) > Vec2::Magnitude(velocity)) velocity = glm::vec2(0.0f);
	else velocity += velocityChange;

	// update position
	position += velocity * dt;
}

void Ball::info()
{
	std::cout << "Position: x = " << position.x << ", y = " << position.y << std::endl;
	std::cout << "Velocity: x = " << velocity.x << ", y = " << velocity.y << std::endl;
}
