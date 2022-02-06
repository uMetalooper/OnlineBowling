#include "Game.h"

Game::Game()
{
	std::string vertexShaderSource = ResourcesManager::getAbsolutePathOf(R"(Shaders\Sphere.vert)");
	std::string fragmentShaderSource = ResourcesManager::getAbsolutePathOf(R"(Shaders\Sphere.frag)");
	shader = Shader(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

	shader.use();
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
	shader.setMat4("projection", proj);

	ball.setPosition(glm::vec2(0.0f, -3.0f));
}

void Game::Update(float dt)
{
	ball.Update(dt);
	ball.info();
}

void Game::Render()
{
	ball.Draw(shader);
	floor.Draw(shader);
}
