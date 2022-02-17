#include "OnlineBowlingServerPCH.h"

void FloorServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}

bool FloorServer::HandleCollisionWithBall(Ball* inBall)
{
	// ignore collision with ball, for now
	(void)inBall;
	return false;
}

FloorServer::FloorServer()
{
}
