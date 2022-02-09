#include "RoboCatPCH.h"

bool NetworkManagerServer::StaticInit(uint16_t inPort)
{
	sInstance = new NetworkManagerServer();
	return sInstance->Init(inPort);
}
