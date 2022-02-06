#include "glm_vec_helper.h"

float Vec3::Magnitude2(glm::vec3 v)
{
	return glm::dot(v, v);
}

float Vec3::Magnitude(glm::vec3 v)
{
	return glm::sqrt(Vec3::Magnitude2(v));
}

glm::vec3 Vec3::Normalize(glm::vec3 v)
{
	return v / Vec3::Magnitude(v);
}

float Vec2::Magnitude2(glm::vec2 v)
{
	return glm::dot(v, v);
}

float Vec2::Magnitude(glm::vec2 v)
{
	return glm::sqrt(Vec2::Magnitude2(v));
}

glm::vec2 Vec2::Normalize(glm::vec2 v)
{
	return v / Vec2::Magnitude(v);
}