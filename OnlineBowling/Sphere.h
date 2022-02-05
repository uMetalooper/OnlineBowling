#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp> // sin, cos
#include <glm/ext/scalar_constants.hpp> // pi
#include <glm/ext/matrix_transform.hpp> // translate
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Sphere
{
public:
	Sphere() : radius(1.0f), sectorCount(36), stackCount(18)
	{
		buildVertices();
		initRenderData();
	}

	~Sphere()
	{

	}

	void DrawSphere(glm::vec3 position);

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

	unsigned int shaderProgram;
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aNormal;\n"
		"layout (location = 2) in vec2 aTexCoord;\n"
		"out vec2 TexCoords;\n"
		"uniform mat4 model"
		"uniform mat4 view"
		"uniform mat4 projection"
		"void main()\n"
		"{\n"
		"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
		"   TexCoords = aTexCoord;\n"
		"}\0";

	const char* fragmentShaderSource = "#version 330 core\n"
		"in vec2 TexCoords;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
		"}\n\0";
};

