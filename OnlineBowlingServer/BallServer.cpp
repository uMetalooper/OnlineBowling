#include "OnlineBowlingServerPCH.h"

void BallServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}

void BallServer::Update()
{
	Ball::Update();
	Vector3 oldLocation = GetLocation();
	Vector3 oldVelocity = GetVelocity();

	ClientProxyPtr client = NetworkManagerServer::sInstance->GetClientProxy(GetPlayerId());
	if (client)
	{
		MoveList& moveList = client->GetUnprocessedMoveList();
		for (const Move& unprocessedMove : moveList)
		{
			const InputState& currentState = unprocessedMove.GetInputState();
			float deltaTime = unprocessedMove.GetDeltaTime();
			ProcessInput(deltaTime, currentState);
			currentState.PrintSelf();
			//SimulateMovement(deltaTime);
		}

		moveList.Clear();
	}

	//HandleShooting();

	if (oldLocation != GetLocation() || oldVelocity != GetVelocity())
	{
		NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), EBRS_Pose);
	}
}

BallServer::BallServer()
{
}
