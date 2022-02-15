#include "OnlineBowlingClientPCH.h"
Drawable::Drawable(GameObject* inGameObject) :
	mGameObject(inGameObject)
{
	RenderManager::sInstance->AddDrawable(this);
}

Drawable::~Drawable()
{
	RenderManager::sInstance->RemoveDrawable(this);
}

void Drawable::addVertex(float x, float y, float z)
{
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);
}

void Drawable::addNormal(float nx, float ny, float nz)
{
	normals.push_back(nx);
	normals.push_back(ny);
	normals.push_back(nz);
}

void Drawable::addTexCoord(float s, float t)
{
	texCoords.push_back(s);
	texCoords.push_back(t);
}

void Drawable::addIndices(unsigned int i1, unsigned int i2, unsigned int i3)
{
	indices.push_back(i1);
	indices.push_back(i2);
	indices.push_back(i3);
}

void Drawable::buildInterleavedVertices()
{
	for (size_t i = 0; i < getVertexCount(); i++)
	{
		interleavedVertices.push_back(vertices[3 * i]);
		interleavedVertices.push_back(vertices[3 * i + 1]);
		interleavedVertices.push_back(vertices[3 * i + 2]);

		interleavedVertices.push_back(normals[3 * i]);
		interleavedVertices.push_back(normals[3 * i + 1]);
		interleavedVertices.push_back(normals[3 * i + 2]);

		interleavedVertices.push_back(texCoords[2 * i]);
		interleavedVertices.push_back(texCoords[2 * i + 1]);
	}
}

void Drawable::initRenderData()
{
	unsigned int VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, getInterleavedVertexSize(), interleavedVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndexSize(), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}