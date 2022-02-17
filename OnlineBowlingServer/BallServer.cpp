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
			if (currentState.IsShooting())
			{
				SetVelocity(Vector3(0.0f, 8.0f, 0.0f));
			}
			//currentState.PrintSelf();
			//SimulateMovement(deltaTime);
		}

		moveList.Clear();
	}

	float sourceRadius = GetCollisionRadius();
	Vector3 sourceLocation = GetLocation();
	for (auto goIt = World::sInstance->GetGameObjects().begin(), end = World::sInstance->GetGameObjects().end(); goIt != end; ++goIt)
	{
		GameObject* target = goIt->get();
		if (target != this && !target->DoesWantToDie())
		{
			Vector3 targetLocation = target->GetLocation();
			float targetRadius = target->GetCollisionRadius();

			Vector3 delta = targetLocation - sourceLocation;
			float dist = glm::length(delta);
			float collisionDist = (sourceRadius + targetRadius);
			if (dist < collisionDist)
			{
				if (target->HandleCollisionWithBall(this))
				{
					//find direction from other ball to this ball
					Ball* targetBall = target->GetAsBall();
					glm::vec2 relDir = glm::normalize(GetLocation() - targetBall->GetLocation());

					//split velocities into 2 parts:  one component perpendicular, and one parallel to 
					//the collision plane, for both balls
					//(NB the collision plane is defined by the point of contact and the contact normal)
					float perpV = glm::dot((glm::vec2)GetVelocity(), relDir);
					float perpV2 = glm::dot((glm::vec2)targetBall->GetVelocity(), relDir);
					glm::vec2 parallelV = (glm::vec2)GetVelocity() - (relDir * perpV);
					glm::vec2 parallelV2 = (glm::vec2)targetBall->GetVelocity() - (relDir * perpV2);

					//Calculate new perpendicluar components:
					//v1 = (2*m2 / m1+m2)*u2 + ((m1 - m2)/(m1+m2))*u1;
					//v2 = (2*m1 / m1+m2)*u1 + ((m2 - m1)/(m1+m2))*u2;
					float perpVNew = perpV2;
					float perpVNew2 = perpV;

					//find new velocities by adding unchanged parallel component to new perpendicluar component
					SetVelocity(Vector3(parallelV + (relDir * perpVNew), 0.0f));
					targetBall->SetVelocity(Vector3(parallelV2 + (relDir * perpVNew2), 0.0f));
				}
			}
		}
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
