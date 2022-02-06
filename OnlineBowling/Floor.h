#pragma once
#include <glm/glm.hpp>
#include "Plane.h"
class Floor
{
public:
	Floor()
	{
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		size = glm::vec2(1.0f);
	}

	void Draw(Shader& shader)
	{
		renderer.setCenter(center);
		renderer.setSize(size);
		renderer.setColor(color);
		renderer.Draw(shader);
	}

	Plane renderer;
	glm::vec3 center;
	glm::vec2 size;
	glm::vec3 color;
};

