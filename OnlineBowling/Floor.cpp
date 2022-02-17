#include "OnlineBowlingPCH.h"

uint32_t Floor::Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const
{
	uint32_t writtenState = 0;

	if (inDirtyState | EFRS_Pose)
	{
		inOutputStream.Write((bool)true);
		Vector3 location = GetLocation();
		inOutputStream.Write(location.x);
		inOutputStream.Write(location.y);
		inOutputStream.Write(location.z);

		Vector3 size = GetSize();
		inOutputStream.Write(size.x);
		inOutputStream.Write(size.y);
		inOutputStream.Write(size.z);

		writtenState |= EFRS_Pose;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	if (inDirtyState | EFRS_Color)
	{
		inOutputStream.Write((bool)true);
		inOutputStream.Write(GetColor());
		writtenState |= EFRS_Color;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}
	return writtenState;
}

bool Floor::HandleCollisionWithBall(Ball* inBall)
{
	(void)inBall;
	return false;
}
