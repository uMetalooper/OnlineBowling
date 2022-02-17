#include "OnlineBowlingServerPCH.h"

uint8_t BallServer::sRemovedIndex = 0;
uint8_t BallServer::sLastRemovedIndex = 0;

void BallServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}

void BallServer::Update()
{
	TinyLogger log("BallServer::Update");
	Vector3 oldLocation = GetLocation();
	Vector3 oldVelocity = GetVelocity();

	int pid = GetPlayerId();
	LOG("Player id: %d", pid);
	ClientProxyPtr client = NetworkManagerServer::sInstance->GetClientProxy(GetPlayerId());
	// player's ball
	if (client)
	{
		MoveList& moveList = client->GetUnprocessedMoveList();
		if (!client->GetIsBallThrown())
		{
			LOG("Ball not thrown", NULL);
			if (!GetActive())
			{
				LOG("Ball not active", NULL);
				moveList.Clear();
			}
			else
			{
				LOG("Ball active", NULL);
			}
			for (const Move& unprocessedMove : moveList)
			{
				const InputState& currentState = unprocessedMove.GetInputState();
				float deltaTime = unprocessedMove.GetDeltaTime();
				ProcessInput(deltaTime, currentState);
				if (currentState.IsShooting() && !client->GetIsBallThrown())
				{
					LOG("Shoot!!!", NULL);
					SetVelocity(Vector3(0.0f, 8.0f, 0.0f));
					client->SetIsBallThrown(true);
				}
			}
			Ball::Update();
			moveList.Clear();
			if (oldLocation != GetLocation() || oldVelocity != GetVelocity())
			{
				NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), EBRS_Pose);
			}
			return;
		}
		else
		{
			LOG("Ball thrown", NULL);
			LOG("Removed Index: %d", sRemovedIndex);
			if (CheckOutOfBound())
			{
				SetVelocity(Vector3(0.0f));
			}
			if (World::sInstance->AllGameObjectsStopMoving())
			{
				LOG("Reset ball", NULL);
				World::sInstance->IncPlayerIndex();
				ScoreBoardManager::sInstance->IncScore(GetPlayerId(), sRemovedIndex - sLastRemovedIndex);
				sLastRemovedIndex = sRemovedIndex;

				// reset ball
				SetLocation(Vector3(0.0f, -1.0f, BALL_RADIUS));
				client->SetIsBallThrown(false);
				if (sRemovedIndex == 10)
				{
					World::sInstance->IncPlayerIndex();
					//sRemovedIndex = 0;
				}
				// reset pins
				if (World::sInstance->GetPlayerIndex() >= 2)
				{
					LOG("Reset pins", NULL);
					World::sInstance->ResetPlayerIndex();
					SetLocation(Vector3(0.0f, -2.0f, BALL_RADIUS));
					SetActive(false);

					sRemovedIndex = 0;
					sLastRemovedIndex = 0;
					int ballIdx = 0;
					for (auto goIt = World::sInstance->GetGameObjects().begin(), end = World::sInstance->GetGameObjects().end();
						goIt != end; ++goIt)
					{
						GameObject* target = goIt->get();
						if (target != this && !target->DoesWantToDie())
						{
							Ball* ball = target->GetAsBall();
							if (ball)
							{
								// transfer control to other player
								auto other = NetworkManagerServer::sInstance->GetClientProxy(ball->GetPlayerId());
								if (other)
								{
									ball->SetLocation(Vector3(0.0f, -1.0f, BALL_RADIUS));
									ball->SetActive(true);

									continue;
								}
								ballIdx++;
								const Vector3 white(1.0f);

								constexpr float sep = 0.25;
								constexpr float rowSep = sep * 0.866;

								ball->SetColor(white);

								// reset position
								int row = 1;
								int rowIndex = ballIdx;
								while (rowIndex > row)
								{
									rowIndex -= row;
									row++;
								}
								float x = (((row - 1) * sep) / 2.0f) - (sep * (row - rowIndex));
								float y = rowSep * (row - 1) + 18.0f - sep * 5;
								ball->SetLocation(Vector3(x, y, 0.1f));
								ball->SetVelocity(Vector3(0.0f));

								ball->SetActive(true);
								ball->SetIsTouched(false);
							}
						}
					}
				}
			}
		}
		// remove redundant moves
		if (moveList.HasMoves())
		{
			moveList.Clear();
		}
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
					SetIsTouched(true);
					//find direction from other ball to this ball
					Ball* targetBall = target->GetAsBall();
					targetBall->SetIsTouched(true);
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

	LOG("Before update", NULL);
	LOG("Ball location: %f, %f, %f", GetLocation().x, GetLocation().y, GetLocation().z);
	LOG("Ball velocity: %f, %f, %f", GetVelocity().x, GetVelocity().y, GetVelocity().z);
	Ball::Update();
	LOG("After update", NULL);
	LOG("Ball location: %f, %f, %f", GetLocation().x, GetLocation().y, GetLocation().z);
	LOG("Ball velocity: %f, %f, %f", GetVelocity().x, GetVelocity().y, GetVelocity().z);
	if (!client)
	{
		LOG("Update Pins", NULL);
		if (GetActive() && (CheckOutOfBound() || (GetIsTouched() && IsStop())))
		{
			uint8_t rowIndex = sRemovedIndex / 3;
			uint8_t colIndex = sRemovedIndex % 3;
			SetActive(false);
			SetColor(glm::vec3(0.0f));
			SetLocation(
				glm::vec3((colIndex - 1) * 3 * BALL_RADIUS,
					ALLEY_LENGTH + (rowIndex + 1) * 3 * BALL_RADIUS,
					BALL_RADIUS));
			SetVelocity(Vector3(0.0f));
			sRemovedIndex++;
		}
	}

	if (oldLocation != GetLocation() || oldVelocity != GetVelocity())
	{
		NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), EBRS_Pose);
	}
}

BallServer::BallServer()
{
}

bool BallServer::CheckOutOfBound()
{
	auto ballPos = GetLocation();
	return ballPos.x > HALF_ALLEY_WIDTH || ballPos.x < -HALF_ALLEY_WIDTH
		|| ballPos.y > ALLEY_LENGTH;
}