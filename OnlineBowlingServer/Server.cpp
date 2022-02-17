#include "OnlineBowlingServerPCH.h"

int Server::sGameId = 0;

void Server::Game::Update()
{
	mBallA->Update();
	mBallB->Update();
	for (int i = 0; i < 10; i++)
	{
		mPins[i]->Update();
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
	// managed game
	GamePtr game = std::make_shared<Game>();

	int gameId = inClientAProxy->GetPlayerId() / 2;
	float gapBetweenGame = 1.2f;
	int playerAId = inClientAProxy->GetPlayerId();
	ScoreBoardManager::sInstance->AddEntry(playerAId, inClientAProxy->GetName());
	BallPtr ballA = std::static_pointer_cast<Ball>(GameObjectRegistry::sInstance->CreateGameObject('BALL'));
	ballA->SetColor(ScoreBoardManager::sInstance->GetEntry(playerAId)->GetColor());
	ballA->SetActive(true);
	ballA->SetPlayerId(playerAId);
	ballA->SetLocation(Vector3(gameId * gapBetweenGame, -1.0f, 0.1f));
	ballA->SetVelocity(Vector3(0.0f));

	game->mBallA = ballA;

	int playerBId = inClientBProxy->GetPlayerId();
	ScoreBoardManager::sInstance->AddEntry(playerBId, inClientBProxy->GetName());
	BallPtr ballB = std::static_pointer_cast<Ball>(GameObjectRegistry::sInstance->CreateGameObject('BALL'));
	ballB->SetColor(ScoreBoardManager::sInstance->GetEntry(playerBId)->GetColor());
	ballB->SetActive(false);
	ballB->SetPlayerId(playerBId);
	ballB->SetLocation(Vector3(gameId * gapBetweenGame, -2.0f, 0.1f));
	ballB->SetVelocity(Vector3(0.0f));

	game->mBallB = ballB;

	FloorPtr floor = std::static_pointer_cast<Floor>(GameObjectRegistry::sInstance->CreateGameObject('FLOO'));
	floor->SetLocation(Vector3(gameId * gapBetweenGame, 9.0f, 0.0f));
	floor->SetSize(Vector3(1.0f, 18.0f, 1.0f));
	floor->SetColor(Vector3(1.0f, 0.0f, 0.0f));

	game->mFloor = floor;

	// Spawn pins
	const Vector3 white(1.0f);

	constexpr float sep = 0.25;
	constexpr float rowSep = sep * 0.866;
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

		game->mPins[i] = pin;
	}

	mIdToGames.insert(std::pair<int, GamePtr>(++sGameId, game));
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
