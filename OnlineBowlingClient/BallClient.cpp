#include "OnlineBowlingClientPCH.h"

void BallClient::Read(InputMemoryBitStream& inInputStream)
{
	bool stateBit;

	uint32_t readState = 0;

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		uint32_t playerId;
		inInputStream.Read(playerId);
		SetPlayerId(playerId);

		readState |= EBRS_PlayerId;
	}

	Vector3 replicatedLocation(0.0f);
	Vector3 replicatedVelocity(0.0f);

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		inInputStream.Read(replicatedVelocity.x);
		inInputStream.Read(replicatedVelocity.y);
		inInputStream.Read(replicatedVelocity.z);
		SetVelocity(replicatedVelocity);

		inInputStream.Read(replicatedLocation.x);
		inInputStream.Read(replicatedLocation.y);
		inInputStream.Read(replicatedLocation.z);
		SetLocation(replicatedLocation);

		readState |= EBRS_Pose;
	}
	
	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Vector3 color;
		inInputStream.Read(color);
		SetColor(color);

		readState |= EBRS_Color;
	}
}