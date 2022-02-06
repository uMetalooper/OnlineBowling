#pragma once
#include <glm/glm.hpp>

namespace Vec3 
{
	float Magnitude2(glm::vec3 v);

	float Magnitude(glm::vec3 v);

	glm::vec3 Normalize(glm::vec3 v);
}

namespace Vec2
{
	float Magnitude2(glm::vec2 v);

	float Magnitude(glm::vec2 v);

	glm::vec2 Normalize(glm::vec2 v);
}