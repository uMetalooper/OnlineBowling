#include "Plane.h"

void Plane::Draw(Shader& shader)
{
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
