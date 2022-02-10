#include "OnlineBowlingServerPCH.h"

void FloorServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}

FloorServer::FloorServer()
{
}
