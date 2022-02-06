#include "Game.h"
//#include "glm_vec_helper.h"

Game::Game()
{
	std::string vertexShaderSource = ResourcesManager::getAbsolutePathOf(R"(Shaders\Sphere.vert)");
	std::string fragmentShaderSource = ResourcesManager::getAbsolutePathOf(R"(Shaders\Sphere.frag)");
	shader = Shader(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

	shader.use();
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
	shader.setMat4("projection", proj);

	floor.size = glm::vec2(ALLEY_WIDTH, ALLEY_LENGTH); // 1 meter wide, 18.288 meters longs
	floor.center = glm::vec3(0.0f, ALLEY_LENGTH / 2.0f, 0.0f);

	ResetBall();
	ResetPins();
}

void Game::ResetBall()
{
	constexpr glm::vec3 green = glm::vec3(0.0f, 1.0f, 0.0f);
	ball.setColor(green);
	ball.setPosition(glm::vec2(0.0f, -1.0f));
	ball.setVelocity(glm::vec2(0.0f));
}

void Game::ResetPins()
{
	constexpr glm::vec3 white = glm::vec3(1.0f);

	constexpr float sep = 0.3;
	constexpr float rowSep = 0.3 * 0.866;
	for (int i = 0; i < 10; i++)
	{
		pins[i].setColor(white);
		int row = 1;
		int rowIndex = i + 1;
		while (rowIndex > row)
		{
			rowIndex -= row;
			row++;
		}
		float x = (((row - 1) * sep) / 2.0f) - (sep * (row - rowIndex));
		float y = rowSep * (row - 1);
		pins[i].setPosition(glm::vec2(x, y));
	}
}

bool Game::CheckCollisions(Ball& one, Ball& two)
{
	glm::vec2 relPosn = one.getPosition() - two.getPosition();
	float dist = Vec2::Magnitude(relPosn);
	glm::vec2 relPosnNorm = Vec2::Normalize(relPosn);
	glm::vec2 relVelocity = one.getVelocity() - two.getVelocity();

	// if moving apart
	if (glm::dot(relVelocity, relPosnNorm) >= 0.0f) return false;
	if (dist > BALL_RADIUS * 2) return false;
	return true;
}

void Game::Update(float dt)
{
	// check collisions
	for (int i = 0; i < 10; i++)
	{
		bool collision = CheckCollisions(ball, pins[i]);
		if (collision)
		{
			//find direction from other ball to this ball
			glm::vec2 relDir = Vec2::Normalize(ball.getPosition() - pins[i].getPosition());

			//split velocities into 2 parts:  one component perpendicular, and one parallel to 
			//the collision plane, for both balls
			//(NB the collision plane is defined by the point of contact and the contact normal)
			float perpV = glm::dot(ball.getVelocity(), relDir);
			float perpV2 = glm::dot(pins[i].getVelocity(), relDir);
			glm::vec2 parallelV = ball.getVelocity() - (relDir * perpV);
			glm::vec2 parallelV2 = pins[i].getVelocity() - (relDir * perpV2);

			//Calculate new perpendicluar components:
			//v1 = (2*m2 / m1+m2)*u2 + ((m1 - m2)/(m1+m2))*u1;
			//v2 = (2*m1 / m1+m2)*u1 + ((m2 - m1)/(m1+m2))*u2;
			float perpVNew = perpV2;
			float perpVNew2 = perpV;

			//find new velocities by adding unchanged parallel component to new perpendicluar component
			ball.setVelocity(parallelV + (relDir * perpVNew));
			pins[i].setVelocity(parallelV2 + (relDir * perpVNew2));
		}
	}

	ball.Update(dt);
	for (int i = 0; i < 10; i++)
	{
		pins[i].Update(dt);
	}
}

void Game::Render()
{
	ball.Draw(shader);
	floor.Draw(shader);
	for (int i = 0; i < 10; i++)
	{
		pins[i].Draw(shader);
	}
}
