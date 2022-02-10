#include "OnlineBowlingClientPCH.h"

void Plane::Draw(Shader& shader)
{
	center = GetGameObject()->GetLocation();
	color = GetGameObject()->GetColor();
	shader.use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, center);

	model = glm::rotate(model, glm::radians(rotateX), right);
	model = glm::rotate(model, glm::radians(rotateY), front);
	model = glm::rotate(model, glm::radians(rotateZ), up);

	model = glm::scale(model, glm::vec3(size, 1.0f));
	shader.setMat4("model", model);
	shader.setVec3("color", color);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);

	// unbind
	glBindVertexArray(0);
}

void Plane::BuildPlane()
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
