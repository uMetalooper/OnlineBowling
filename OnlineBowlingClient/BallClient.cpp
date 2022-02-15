#include "OnlineBowlingClientPCH.h"

void BallClient::Read(InputMemoryBitStream& inInputStream)
{
	uint32_t playerId;
	inInputStream.Read(playerId);
	SetPlayerId(playerId);

	Vector3 replicatedLocation(0.0f);
	Vector3 replicatedVelocity(0.0f);

	inInputStream.Read(replicatedVelocity.x);
	inInputStream.Read(replicatedVelocity.y);
	SetVelocity(replicatedVelocity);

	inInputStream.Read(replicatedLocation.x);
	inInputStream.Read(replicatedLocation.y);
	SetLocation(replicatedLocation);

	Vector3 color;
	inInputStream.Read(color);
	SetColor(color);
}