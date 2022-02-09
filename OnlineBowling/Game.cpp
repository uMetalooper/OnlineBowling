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

	mainCamera = Camera(ball.getPosition3d());
	setCamera(mainCamera.calcViewMatrix());

	playerIndex = 0;
	timer = 0.0f;
}

void Game::ResetBall()
{
	constexpr glm::vec3 green = glm::vec3(0.0f, 1.0f, 0.0f);
	ball.setColor(green);
	ball.setPosition(glm::vec2(0.0f, -1.0f));
	ball.setVelocity(glm::vec2(0.0f));

	mainCamera.Update(ball.getPosition3d());
	setCamera(mainCamera.calcViewMatrix());
	ballThrown = false;
}

void Game::ResetPins()
{
	constexpr glm::vec3 white = glm::vec3(1.0f);

	constexpr float sep = 0.25;
	constexpr float rowSep = sep * 0.866;
	for (int i = 0; i < 10; i++)
	{
		// reset color
		pins[i].setColor(white);

		// reset position
		int row = 1;
		int rowIndex = i + 1;
		while (rowIndex > row)
		{
			rowIndex -= row;
			row++;
		}
		float x = (((row - 1) * sep) / 2.0f) - (sep * (row - rowIndex));
		float y = rowSep * (row - 1) + ALLEY_LENGTH - sep * 5;
		pins[i].setPosition(glm::vec2(x, y));

		// reset state
		touchedIndex[i] = false;
		pins[i].SetActive(true);
	}
	removedIndex = 0;
}

bool Game::CheckCollisions(Ball& one, Ball& two)
{
	glm::vec2 pos1 = one.getPosition();
	glm::vec2 pos2 = two.getPosition();

	float dist = glm::distance(pos1, pos2);
	if (dist > BALL_RADIUS * 2) return false;

	glm::vec2 relPosn = pos1 - pos2;
	glm::vec2 relVelocity = one.getVelocity() - two.getVelocity();

	// if moving apart
	if (glm::dot(relVelocity, relPosn) >= 0.0f) return false;
	return true;
}

void Game::ApplyCollision(Ball& one, Ball& two)
{
	//find direction from other ball to this ball
	glm::vec2 relDir = glm::normalize(one.getPosition() - two.getPosition());

	//split velocities into 2 parts:  one component perpendicular, and one parallel to 
	//the collision plane, for both balls
	//(NB the collision plane is defined by the point of contact and the contact normal)
	float perpV = glm::dot(one.getVelocity(), relDir);
	float perpV2 = glm::dot(two.getVelocity(), relDir);
	glm::vec2 parallelV = one.getVelocity() - (relDir * perpV);
	glm::vec2 parallelV2 = two.getVelocity() - (relDir * perpV2);

	//Calculate new perpendicluar components:
	//v1 = (2*m2 / m1+m2)*u2 + ((m1 - m2)/(m1+m2))*u1;
	//v2 = (2*m1 / m1+m2)*u1 + ((m2 - m1)/(m1+m2))*u2;
	float perpVNew = perpV2;
	float perpVNew2 = perpV;

	//find new velocities by adding unchanged parallel component to new perpendicluar component
	one.setVelocity(parallelV + (relDir * perpVNew));
	two.setVelocity(parallelV2 + (relDir * perpVNew2));
}

void Game::Update(float dt)
{
	// if the ball has not thrown yet, just update the ball's position
	if (!ballThrown)
	{
		timer += dt;
		ball.setPosition(glm::vec2(glm::sin(timer) / 2.0f, -1.0f));
		return;
	}

	// check collisions
	for (int i = 0; i < 10; i++)
	{
		bool collision = CheckCollisions(ball, pins[i]);
		if (collision)
		{
			ApplyCollision(ball, pins[i]);
			touchedIndex[i] = true;
		}
		for (int j = i + 1; j < 10; j++)
		{
			bool rs = CheckCollisions(pins[i], pins[j]);
			if (rs)
			{
				ApplyCollision(pins[i], pins[j]);
				touchedIndex[i] = true;
				touchedIndex[j] = true;
			}
		}
	}

	ball.Update(dt);
	mainCamera.Update(ball.getPosition3d());
	setCamera(mainCamera.calcViewMatrix());

	for (int i = 0; i < 10; i++)
	{
		if (pins[i].GetActive())
		{
			pins[i].Update(dt);
			glm::vec2 pinPos = pins[i].getPosition();
			// if this pin is moving out of bound OR has been touched and stop moving
			if (CheckBound(pins[i])	|| (touchedIndex[i] && pins[i].IsStop()))
			{
				int rowIndex = removedIndex / 3;
				int colIndex = removedIndex % 3;
				pins[i].SetActive(false);
				pins[i].setColor(glm::vec3(0.0f));
				pins[i].setPosition(glm::vec2((colIndex - 1) * 3 * BALL_RADIUS, ALLEY_LENGTH + (rowIndex + 1) * 3 * BALL_RADIUS));
				pins[i].setVelocity(glm::vec2(0.0f));
				removedIndex++;
			}
		}
	}
	
	if (CheckBound(ball))
	{
		ball.setVelocity(glm::vec2(0.0f));
	}
	for (int i = 0; i < 10; i++)
	{
		if (!pins[i].IsStop() && touchedIndex[i])
		{
			return;
		}
	}
	if (!ball.IsStop())
	{
		return;
	}
	ResetBall();
	playerIndex++;
	if (playerIndex % 2 == 0)
	{
		// each player's second turn has ended
		ResetPins();
		if (playerIndex % 4 == 0)
		{
			// one frame of 2 players has ended
			playerIndex = 0;
		}
	}
}

void Game::Render()
{
	ball.Draw(shader);
	for (int i = 0; i < 10; i++)
	{
		if (true)
		{
			pins[i].Draw(shader);
		}
	}
	floor.Draw(shader);
}
