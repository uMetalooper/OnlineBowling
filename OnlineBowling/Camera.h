#pragma once
#include <glm/glm.hpp>
class Camera
{
public:
	Camera()
	{
		Update(glm::vec3(0.0f));
	}

	Camera(glm::vec3 ballPos)
	{
		Update(ballPos);
	}

	void Update(glm::vec3 ballPos)
	{
		targetPos = glm::vec3(0.0f, ballPos.y + 2.0f, 0.0f);  // in front of the ball 2m
		cameraPos = glm::vec3(0.0f, ballPos.y - 2.0f, 2.0f); // behind the ball 2m, above 2m
		up = glm::vec3(0.0f, 0.0f, 1.0f);
	}

	glm::mat4 calcViewMatrix()
	{
		return glm::lookAt(cameraPos, targetPos, up);
	}

private:
	glm::vec3 targetPos;
	glm::vec3 cameraPos;
	glm::vec3 up;
};