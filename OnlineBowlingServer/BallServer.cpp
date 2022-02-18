#include "OnlineBowlingServerPCH.h"

uint8_t BallServer::sRemovedIndex = 0;
uint8_t BallServer::sLastRemovedIndex = 0;

void BallServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}

void BallServer::Update()
{
	Ball::Update();
	NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), EBRS_Pose);
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