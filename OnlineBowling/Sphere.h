#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp> // sin, cos
#include <glm/ext/scalar_constants.hpp> // pi
#include <glm/ext/matrix_transform.hpp> // translate
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Shader.h"
#include "Drawable.h"

class Sphere : public Drawable
{
public:
	Sphere() 
		: color(0.0f)
	{
		buildVertices();
		initRenderData();
	}

	~Sphere()
	{
	}

	void setPosition(glm::vec3 pos) { position = pos; }
	void setColor(glm::vec3 c) { color = c; }
	void setRadius(float r) { radius = r; }

	void Draw(Shader& shader);

private:
	void buildVertices();

	glm::vec3 position;
	glm::vec3 color;

	float radius;
};

