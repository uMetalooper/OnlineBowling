#include "OnlineBowlingClientPCH.h"

constexpr float ALLEY_WIDTH = 1.0f; // 1 meter wide
constexpr float ALLEY_LENGTH = 18.288f; // 18.288 meters long

constexpr float BALL_RADIUS = 0.1;

Game::Game()
{
	std::string vertexShaderSource = ResourcesManager::getAbsolutePathOf(R"(Shaders\Sphere.vert)");
	std::string fragmentShaderSource = ResourcesManager::getAbsolutePathOf(R"(Shaders\Sphere.frag)");
	shader = Shader(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

	shader.use();
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
	shader.setMat4("projection", proj);

	/*floor.size = glm::vec2(ALLEY_WIDTH, ALLEY_LENGTH); 
	floor.center = glm::vec3(0.0f, ALLEY_LENGTH / 2.0f, 0.0f);*/

	ResetBall();
	ResetPins();

	mainCamera = Camera(ball.GetLocation());
	setCamera(mainCamera.calcViewMatrix());

	playerIndex = 0;
	timer = 0.0f;
}

void Game::ResetBall()
{
	const Vector3 green = Vector3(0.0f, 1.0f, 0.0f);
	ball.SetColor(green);
	ball.SetLocation(Vector3(0.0f, -1.0f, BALL_RADIUS));
	ball.SetVelocity(Vector3(0.0f));

	mainCamera.Update(ball.GetLocation());
	setCamera(mainCamera.calcViewMatrix());
	ballThrown = false;
}

void Game::ResetPins()
{
	const Vector3 white(1.0f);

	constexpr float sep = 0.25;
	constexpr float rowSep = sep * 0.866;
	for (int i = 0; i < 10; i++)
	{
		// reset color
		pins[i].SetColor(white);

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
		pins[i].SetLocation(Vector3(x, y, BALL_RADIUS));

		// reset state
		touchedIndex[i] = false;
		pins[i].SetActive(true);
	}
	removedIndex = 0;
}

bool Game::CheckCollisions(Ball& one, Ball& two)
{
	glm::vec2 pos1 = one.GetLocation();
	glm::vec2 pos2 = two.GetLocation();

	float dist = glm::distance(pos1, pos2);
	if (dist > BALL_RADIUS * 2) return false;

	glm::vec2 relPosn = pos1 - pos2;
	glm::vec2 relVelocity = one.GetVelocity() - two.GetVelocity();

	// if moving apart
	if (glm::dot(relVelocity, relPosn) >= 0.0f) return false;
	return true;
}

void Game::ApplyCollision(Ball& one, Ball& two)
{
	//find direction from other ball to this ball
	glm::vec2 relDir = glm::normalize(one.GetLocation() - two.GetLocation());

	//split velocities into 2 parts:  one component perpendicular, and one parallel to 
	//the collision plane, for both balls
	//(NB the collision plane is defined by the point of contact and the contact normal)
	float perpV = glm::dot((glm::vec2)one.GetVelocity(), relDir);
	float perpV2 = glm::dot((glm::vec2)two.GetVelocity(), relDir);
	glm::vec2 parallelV = (glm::vec2)one.GetVelocity() - (relDir * perpV);
	glm::vec2 parallelV2 = (glm::vec2)two.GetVelocity() - (relDir * perpV2);

	//Calculate new perpendicluar components:
	//v1 = (2*m2 / m1+m2)*u2 + ((m1 - m2)/(m1+m2))*u1;
	//v2 = (2*m1 / m1+m2)*u1 + ((m2 - m1)/(m1+m2))*u2;
	float perpVNew = perpV2;
	float perpVNew2 = perpV;

	//find new velocities by adding unchanged parallel component to new perpendicluar component
	/*one.SetVelocity(parallelV + (relDir * perpVNew));
	two.SetVelocity(parallelV2 + (relDir * perpVNew2));*/
}

bool Game::CheckBound(Ball b)
{
	glm::vec2 ballPos = b.GetLocation();
	return ballPos.x > ALLEY_WIDTH / 2.0f || ballPos.x < -ALLEY_WIDTH / 2.0f
		|| ballPos.y > ALLEY_LENGTH;
}

void Game::Update(float dt)
{
	//float dt = Timing::sInstance.GetDeltaTime();
	auto& moveList = InputManager::sInstance->GetMoveList();
	// if the ball has not thrown yet, just update the ball's position
	if (!ballThrown)
	{
		for (const Move& move : moveList)
		{
			const InputState& currentState = move.GetInputState();
			float deltaTime = move.GetDeltaTime();
			ball.ProcessInput(deltaTime, currentState);
			if (currentState.IsShooting())
			{
				applyImpulse(glm::vec2(0.0f, 8.0f));
			}
		}
		moveList.Clear();
		ball.Update();
		return;
	}

	// clear redundant moves after throwing
	if (moveList.HasMoves())
	{
		moveList.Clear();
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

	ball.Update();
	mainCamera.Update(ball.GetLocation());
	setCamera(mainCamera.calcViewMatrix());

	for (int i = 0; i < 10; i++)
	{
		if (pins[i].GetActive())
		{
			pins[i].Update();
			glm::vec2 pinPos = pins[i].GetLocation();
			// if this pin is moving out of bound OR has been touched and stop moving
			if (CheckBound(pins[i]) || (touchedIndex[i] && pins[i].IsStop()))
			{
				int rowIndex = removedIndex / 3;
				int colIndex = removedIndex % 3;
				pins[i].SetActive(false);
				pins[i].SetColor(glm::vec3(0.0f));
				pins[i].SetLocation(
					glm::vec3((colIndex - 1) * 3 * BALL_RADIUS, 
						ALLEY_LENGTH + (rowIndex + 1) * 3 * BALL_RADIUS, 
						BALL_RADIUS));
				pins[i].SetVelocity(Vector3(0.0f));
				removedIndex++;
			}
		}
	}

	if (CheckBound(ball))
	{
		ball.SetVelocity(Vector3(0.0f));
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
	/*ball.Draw(shader);
	for (int i = 0; i < 10; i++)
	{
		if (true)
		{
			pins[i].Draw(shader);
		}
	}
	floor.Draw(shader);*/
}
