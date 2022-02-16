#include "OnlineBowlingClientPCH.h"

void FloorClient::Read(InputMemoryBitStream& inInputStream)
{
	bool stateBit;

	uint32_t readState = 0;

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Vector3 replicatedLocation;
		inInputStream.Read(replicatedLocation.x);
		inInputStream.Read(replicatedLocation.y);
		inInputStream.Read(replicatedLocation.z);
		SetLocation(replicatedLocation);

		Vector3 replicatedSize;
		inInputStream.Read(replicatedSize.x);
		inInputStream.Read(replicatedSize.y);
		inInputStream.Read(replicatedSize.z);
		SetSize(replicatedSize);

		readState |= EFRS_Pose;
	}
	
	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Vector3 color;
		inInputStream.Read(color);
		SetColor(color);

		readState |= EFRS_Color;
	}
}
