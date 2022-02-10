#include "OnlineBowlingClientPCH.h"

void Sphere::buildVertices()
{
	const unsigned int sectorCount = 36;
	const unsigned int stackCount = 18;
	float x, y, z, xy; // vertex position
	float nx, ny, nz;
	float s, t;

	float sectorStep = 2 * glm::pi<float>() / sectorCount;
	float stackStep = glm::pi<float>() / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i < stackCount; i++)
	{
		stackAngle = glm::pi<float>() / 2 - i * stackStep;
		xy = 1.0f * glm::cos(stackAngle);
		z = 1.0f * glm::sin(stackAngle);
		for (int j = 0; j < sectorCount; j++)
		{
			sectorAngle = j * sectorStep;

			x = xy * glm::cos(sectorAngle);
			y = xy * glm::sin(sectorAngle);
			addVertex(x, y, z);

			// normalize normal vector
			nx = x / 1.0f;
			ny = y / 1.0f;
			nz = z / 1.0f;
			addNormal(nx, ny, nz);

			// add texture coordinate
			s = (float)i / stackCount;
			t = (float)j / sectorCount;
			addTexCoord(s, t);
		}
	}

	// indices
	//  k1--k1+1
	//  |  / |
	//  | /  |
	//  k2--k2+1
	unsigned int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding 1st and last stacks
			if (i != 0)
			{
				addIndices(k1, k2, k1 + 1);   // k1---k2---k1+1
			}

			if (i != (stackCount - 1))
			{
				addIndices(k1 + 1, k2, k2 + 1); // k1+1---k2---k2+1
			}
		}
	}

	buildInterleavedVertices();
}



void Sphere::Draw(Shader& shader)
{
	position = GetGameObject()->GetLocation();
	color = GetGameObject()->GetColor();
	radius = 0.1f;

	shader.use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);

	// scale
	model = glm::scale(model, glm::vec3(radius));
	shader.setMat4("model", model);
	shader.setVec3("color", color);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);

	// unbind
	glBindVertexArray(0);
}