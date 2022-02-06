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

class Sphere
{
public:
	Sphere() 
		: radius(1.0f), sectorCount(36), stackCount(18)
	{
		buildVertices();
		initRenderData();
	}

	~Sphere()
	{
	}

	void DrawSphere(Shader& shader, glm::vec3 position);

	uint32_t getVertexCount() const { return (uint32_t)vertices.size() / 3; }
	uint32_t getInterleavedVertexSize() const { return (uint32_t)interleavedVertices.size() * sizeof(float); }
	uint32_t getIndexSize() const { return (uint32_t)indices.size() * sizeof(uint32_t); }

private:
	void buildVertices();
	void initRenderData();

	void addVertex(float x, float y, float z);
	void addNormal(float nx, float ny, float nz);
	void addTexCoord(float s, float t);

	void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);

	float radius;
	int sectorCount;
	int stackCount;

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<unsigned int> indices;

	std::vector<float> interleavedVertices;

	unsigned int VAO;
};

