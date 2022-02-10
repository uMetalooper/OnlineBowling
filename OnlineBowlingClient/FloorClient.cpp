#include "OnlineBowlingClientPCH.h"

void FloorClient::Read(InputMemoryBitStream& inInputStream)
{
	Vector3 replicatedLocation;
	inInputStream.Read(replicatedLocation.x);
	inInputStream.Read(replicatedLocation.y);
	SetLocation(replicatedLocation);

	Vector3 color;
	inInputStream.Read(color);
	SetColor(color);
}
