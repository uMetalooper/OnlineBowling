#include "OnlineBowlingServerPCH.h"

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

	Engine::DoFrame();

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

void Server::HandleLostClient(ClientProxyPtr inClientProxy)
{
	//kill client's cat
	//remove client from scoreboard
	int playerId = inClientProxy->GetPlayerId();

	//ScoreBoardManager::sInstance->RemoveEntry(playerId);
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
	//gotta pick a better spawn location than this...
	ball->SetLocation(Vector3(0.0f, 0.0f, 0.0f));
	ball->SetVelocity(Vector3(0.0f));
}
