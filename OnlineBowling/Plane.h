#pragma once
#include <vector>

#include "Shader.h"
#include "Drawable.h"

class Plane : public Drawable
{
public:
	Plane()
		: center(0.0f), size(1.0f), rotateX(0.0f), rotateY(0.0f), rotateZ(0.0f), color(0.0f)
	{
		up = glm::vec3(0.0f, 0.0f, 1.0f);
		right = glm::vec3(1.0f, 0.0f, 0.0f);
		front = glm::cross(up, right);

		glm::vec3 tl = center - right * size.x / 2.0f + front * size.y / 2.0f; // 0:top-left
		glm::vec3 tr = center + right * size.x / 2.0f + front * size.y / 2.0f; // 1:top-right
		glm::vec3 bl = center - right * size.x / 2.0f - front * size.y / 2.0f; // 2:bottom-left
		glm::vec3 br = center + right * size.x / 2.0f - front * size.y / 2.0f; // 3:bottom-right
		addVertex(tl.x, tl.y, tl.z);
		addNormal(up.x, up.y, up.z);
		addTexCoord(0.0f, 0.0f);

		addVertex(tr.x, tr.y, tr.z);
		addNormal(up.x, up.y, up.z);
		addTexCoord(0.0f, 1.0f);

		addVertex(bl.x, bl.y, bl.z);
		addNormal(up.x, up.y, up.z);
		addTexCoord(1.0f, 0.0f);

		addVertex(br.x, br.y, br.z);
		addNormal(up.x, up.y, up.z);
		addTexCoord(1.0f, 1.0f);

		addIndices(0, 1, 2);
		addIndices(1, 2, 3);

		buildInterleavedVertices();
		initRenderData();
	}

	void setCenter(glm::vec3 newCenter)
	{
		center = newCenter;
	}

	void setSize(glm::vec2 newSize)
	{
		size = newSize;
	}

	void setRotate(glm::vec3 newRotate)
	{
		rotateX = newRotate.x;
		rotateY = newRotate.y;
		rotateZ = newRotate.z;
	}

	void setColor(glm::vec3 newColor)
	{
		color = newColor;
	}

	void Draw(Shader& shader);

private:
	glm::vec3 center;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 front;
	float rotateX;
	float rotateY;
	float rotateZ;
	glm::vec2 size;

	glm::vec3 color;
};

