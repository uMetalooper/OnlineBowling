#include "OnlineBowlingServerPCH.h"



void Server::Game::ResetBall()
{
	//const Vector3 green = Vector3(0.0f, 1.0f, 0.0f);
	//const Vector3 blue = Vector3(0.0f, 0.0f, 1.0f);
	//mBallA->SetColor(green);
	mBallA->SetVelocity(Vector3(0.0f));
	//mBallB->SetColor(blue);
	mBallB->SetVelocity(Vector3(0.0f));
	if ((mPlayerIndex / 2) == 0)
	{
		mActiveBall = mBallA;
		mBallA->SetLocation(Vector3(mGameOffsetX, -1.0f, BALL_RADIUS));
		mBallB->SetLocation(Vector3(mGameOffsetX, -2.0f, BALL_RADIUS));
	}
	else
	{
		mActiveBall = mBallB;
		mBallB->SetLocation(Vector3(mGameOffsetX, -1.0f, BALL_RADIUS));
		mBallA->SetLocation(Vector3(mGameOffsetX, -2.0f, BALL_RADIUS));
	}

	//mainCamera.Update(ball.GetLocation());
	//setCamera(mainCamera.calcViewMatrix());
	mBallThrown = false;
	mPoint = 0;
}

void Server::Game::ResetPins()
{
	const Vector3 white(1.0f);

	constexpr float sep = 0.25;
	constexpr float rowSep = sep * 0.866;
	for (int i = 0; i < 10; i++)
	{
		// reset color
		mPins[i]->SetColor(white);

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
		mPins[i]->SetLocation(Vector3(mGameOffsetX + x, y, BALL_RADIUS));

		// reset state
		mTouchedIndex[i] = false;
		mPins[i]->SetActive(true);
	}
	mRemovedIndex = 0;
}

bool Server::Game::CheckCollisions(BallPtr one, BallPtr two)
{
	glm::vec2 pos1 = one->GetLocation();
	glm::vec2 pos2 = two->GetLocation();

	float dist = glm::distance(pos1, pos2);
	if (dist > BALL_RADIUS * 2) return false;

	glm::vec2 relPosn = pos1 - pos2;
	glm::vec2 relVelocity = one->GetVelocity() - two->GetVelocity();

	// if moving apart
	if (glm::dot(relVelocity, relPosn) >= 0.0f) return false;
	return true;
}

void Server::Game::CollisionResponse(BallPtr one, BallPtr two)
{
	//find direction from other ball to this ball
	glm::vec2 relDir = glm::normalize(one->GetLocation() - two->GetLocation());

	//split velocities into 2 parts:  one component perpendicular, and one parallel to 
	//the collision plane, for both balls
	//(NB the collision plane is defined by the point of contact and the contact normal)
	float perpV = glm::dot((glm::vec2)one->GetVelocity(), relDir);
	float perpV2 = glm::dot((glm::vec2)two->GetVelocity(), relDir);
	glm::vec2 parallelV = (glm::vec2)one->GetVelocity() - (relDir * perpV);
	glm::vec2 parallelV2 = (glm::vec2)two->GetVelocity() - (relDir * perpV2);

	//Calculate new perpendicluar components:
	//v1 = (2*m2 / m1+m2)*u2 + ((m1 - m2)/(m1+m2))*u1;
	//v2 = (2*m1 / m1+m2)*u1 + ((m2 - m1)/(m1+m2))*u2;
	float perpVNew = perpV2;
	float perpVNew2 = perpV;

	//find new velocities by adding unchanged parallel component to new perpendicluar component
	one->SetVelocity(Vector3(parallelV + (relDir * perpVNew), 0.0f));
	two->SetVelocity(Vector3(parallelV2 + (relDir * perpVNew2), 0.0f));
}

bool Server::Game::CheckBound(BallPtr ball)
{
	// TODO: Adding game offset
	Vector3 ballPos = ball->GetLocation();
	return ballPos.x > mGameOffsetX + HALF_ALLEY_WIDTH || ballPos.x < mGameOffsetX - HALF_ALLEY_WIDTH
		|| ballPos.y > ALLEY_LENGTH;
}

void Server::Game::Update()
{
	//mActiveBall = mBallA;
	ClientProxyPtr client = NetworkManagerServer::sInstance->GetClientProxy(mActiveBall->GetPlayerId());
	MoveList& moveList = client->GetUnprocessedMoveList();
	if (!mBallThrown)
	{
		for (const Move& unprocessedMove : moveList)
		{
			const InputState& currentState = unprocessedMove.GetInputState();
			float deltaTime = unprocessedMove.GetDeltaTime();

			float inputDelta = currentState.GetDesiredHorizontalDelta();
			Vector3 oldLocation = mActiveBall->GetLocation();
			Vector3 newLocation = oldLocation + Vector3(inputDelta * deltaTime, 0.0f, 0.0f);
			if (newLocation.x < mGameOffsetX - HALF_ALLEY_WIDTH)
			{
				mActiveBall->SetLocation(Vector3(mGameOffsetX - HALF_ALLEY_WIDTH, newLocation.y, newLocation.z));
			}
			else if (newLocation.x > mGameOffsetX + HALF_ALLEY_WIDTH)
			{
				mActiveBall->SetLocation(Vector3(mGameOffsetX + HALF_ALLEY_WIDTH, newLocation.y, newLocation.z));
			}
			else
			{
				mActiveBall->SetLocation(newLocation);
			}
			if (currentState.IsShooting())
			{
				mActiveBall->SetVelocity(Vector3(0.0f, 8.0f, 0.0f));
				mBallThrown = true;
			}
		}
		moveList.Clear();
		mActiveBall->Update();
		return;
	}

	// clear redundant moves after throwing
	if (moveList.HasMoves())
	{
		moveList.Clear();
	}

	// check for collisions
	for (int i = 0; i < 10; i++)
	{
		bool collision = CheckCollisions(mActiveBall, mPins[i]);
		if (collision)
		{
			CollisionResponse(mActiveBall, mPins[i]);
			mTouchedIndex[i] = true;
		}
		for (int j = i + 1; j < 10; j++)
		{
			bool rs = CheckCollisions(mPins[i], mPins[j]);
			if (rs)
			{
				CollisionResponse(mPins[i], mPins[j]);
				mTouchedIndex[i] = true;
				mTouchedIndex[j] = true;
			}
		}
	}
	mBallA->Update();
	mBallB->Update();
	for (int i = 0; i < 10; i++)
	{
		NetworkManagerServer::sInstance->SetStateDirty(mPins[i]->GetNetworkId(), Ball::EBRS_Pose);
		if (mPins[i]->GetActive())
		{
			mPins[i]->Update();
			if (CheckBound(mPins[i]) || (mTouchedIndex[i] && mPins[i]->IsStop()))
			{
				int rowIndex = mRemovedIndex / 3;
				int colIndex = mRemovedIndex % 3;
				mPins[i]->SetActive(false);
				mPins[i]->SetColor(glm::vec3(0.0f));
				mPins[i]->SetLocation(
					glm::vec3(mGameOffsetX + (colIndex - 1) * 3 * BALL_RADIUS,
						ALLEY_LENGTH + (rowIndex + 1) * 3 * BALL_RADIUS,
						BALL_RADIUS));
				mPins[i]->SetVelocity(Vector3(0.0f));
				mRemovedIndex++;
				mPoint++;
			}
		}
	}

	if (CheckBound(mActiveBall))
	{
		mActiveBall->SetVelocity(Vector3(0.0f));
	}
	for (int i = 0; i < 10; i++)
	{
		if (!mPins[i]->IsStop() && mTouchedIndex[i])
		{
			return;
		}
	}
	if (!mActiveBall->IsStop())
	{
		return;
	}

	mPlayerIndex++;
	ScoreBoardManager::sInstance->IncScore(mActiveBall->GetPlayerId(), mPoint);
	if (mRemovedIndex == 10)
	{
		// skip turn if strike, double bonus
		mPlayerIndex++;
		ScoreBoardManager::sInstance->IncScore(mActiveBall->GetPlayerId(), mPoint);
	}
	ResetBall();
	if (mPlayerIndex % 2 == 0)
	{
		ResetPins();
		if (mPlayerIndex % 4 == 0)
		{
			mPlayerIndex = 0;
			ResetBall();
		}
	}
}

bool Server::StaticInit()
{
	sInstance.reset(new Server());
	return true;
}

void Server::DoFrame()
{
	NetworkManagerServer::sInstance->ProcessIncomingPackets();

	NetworkManagerServer::sInstance->CheckForDisconnects();

	//NetworkManagerServer::sInstance->RespawnCats();

	//NetworkManagerServer::sInstance->UpdatePlayerTurn();

	//Engine::DoFrame();
	//World::sInstance->Update();
	for (const auto& pair : mIdToGames)
	{
		pair.second->Update();
	}
	ScoreBoardManager::sInstance->SortEntriesByScore();

	NetworkManagerServer::sInstance->SendOutgoingPackets();
}

Server::Server()
{
	GameObjectRegistry::sInstance->RegisterCreationFunction('BALL', BallServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('FLOO', FloorServer::StaticCreate);

	InitNetworkManager();
}

bool Server::InitNetworkManager()
{
	string portString = StringUtils::GetCommandLineArg(1);
	uint16_t port = stoi(portString);

	return NetworkManagerServer::StaticInit(port);
}

void Server::HandleNewClient(ClientProxyPtr inClientProxy)
{

	int playerId = inClientProxy->GetPlayerId();

	//ScoreBoardManager::sInstance->AddEntry(playerId, inClientProxy->GetName());
	SpawnBallForPlayer(playerId);
}

void Server::HandleNewGame(ClientProxyPtr inClientAProxy, ClientProxyPtr inClientBProxy)
{
	//int gameId = inClientAProxy->GetPlayerId() / 2;
	float gapBetweenGame = 1.2f;
	int playerAId = inClientAProxy->GetPlayerId();
	ScoreBoardManager::sInstance->AddEntry(playerAId, inClientAProxy->GetName());
	BallPtr ballA = std::static_pointer_cast<Ball>(GameObjectRegistry::sInstance->CreateGameObject('BALL'));
	ballA->SetColor(ScoreBoardManager::sInstance->GetEntry(playerAId)->GetColor());
	ballA->SetActive(true);
	ballA->SetPlayerId(playerAId);
	//ballA->SetLocation(Vector3(gameId * gapBetweenGame, -1.0f, 0.1f));
	ballA->SetVelocity(Vector3(0.0f));

	int playerBId = inClientBProxy->GetPlayerId();
	ScoreBoardManager::sInstance->AddEntry(playerBId, inClientBProxy->GetName());
	BallPtr ballB = std::static_pointer_cast<Ball>(GameObjectRegistry::sInstance->CreateGameObject('BALL'));
	ballB->SetColor(ScoreBoardManager::sInstance->GetEntry(playerBId)->GetColor());
	ballB->SetActive(false);
	ballB->SetPlayerId(playerBId);
	//ballB->SetLocation(Vector3(gameId * gapBetweenGame, -2.0f, 0.1f));
	ballB->SetVelocity(Vector3(0.0f));

	FloorPtr floorPtr = std::static_pointer_cast<Floor>(GameObjectRegistry::sInstance->CreateGameObject('FLOO'));
	//floor->SetLocation(Vector3(gameId * gapBetweenGame, 9.0f, 0.0f));
	floorPtr->SetSize(Vector3(1.0f, 18.0f, 1.0f));
	floorPtr->SetColor(Vector3(1.0f, 0.0f, 0.0f));

	// Spawn pins
	const Vector3 white(1.0f);

	constexpr float sep = 0.25;
	constexpr float rowSep = sep * 0.866;
	BallPtr pins[10];
	for (int i = 0; i < 10; i++)
	{
		// reset color
		BallPtr pin = std::static_pointer_cast<Ball>(GameObjectRegistry::sInstance->CreateGameObject('BALL'));
		pin->SetColor(white);

		// reset position
		int row = 1;
		int rowIndex = i + 1;
		while (rowIndex > row)
		{
			rowIndex -= row;
			row++;
		}
		float x = (((row - 1) * sep) / 2.0f) - (sep * (row - rowIndex));
		float y = rowSep * (row - 1) + 18.0f - sep * 5;
		pin->SetLocation(Vector3(x, y, 0.1f));
		pin->SetVelocity(Vector3(0.0f));

		pins[i] = pin;
	}
	int gameId = GetNewGameId();
	GamePtr game = std::make_shared<Game>(gameId, ballA, ballB, pins, floorPtr);
	mIdToGames.insert(std::pair<int, GamePtr>(gameId, game));
}

void Server::HandleLostClient(ClientProxyPtr inClientProxy)
{
	//kill client's cat
	//remove client from scoreboard
	int playerId = inClientProxy->GetPlayerId();

	ScoreBoardManager::sInstance->RemoveEntry(playerId);
	BallPtr ball = GetBallForPlayer(playerId);
	if (ball)
	{
		ball->SetDoesWantToDie(true);
	}
}

BallPtr Server::GetBallForPlayer(int inPlayerId)
{
	//run through the objects till we find the cat...
	//it would be nice if we kept a pointer to the cat on the clientproxy
	//but then we'd have to clean it up when the cat died, etc.
	//this will work for now until it's a perf issue
	const auto& gameObjects = World::sInstance->GetGameObjects();
	for (int i = 0, c = gameObjects.size(); i < c; ++i)
	{
		GameObjectPtr go = gameObjects[i];
		Ball* cat = go->GetAsBall();
		if (cat && cat->GetPlayerId() == inPlayerId)
		{
			return std::static_pointer_cast<Ball>(go);
		}
	}

	return nullptr;
}

void Server::SpawnBallForPlayer(int inPlayerId)
{
	BallPtr ball = std::static_pointer_cast<Ball>(GameObjectRegistry::sInstance->CreateGameObject('BALL'));
	//cat->SetColor(ScoreBoardManager::sInstance->GetEntry(inPlayerId)->GetColor());
	ball->SetPlayerId(inPlayerId);
	ball->SetLocation(Vector3(0.0f, -1.0f, 0.1f));
	ball->SetVelocity(Vector3(0.0f));
}

void Server::SpawnPinsForNewGame()
{

}
