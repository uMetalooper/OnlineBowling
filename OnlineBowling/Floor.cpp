#include "OnlineBowlingPCH.h"

uint32_t Floor::Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const
{
	Vector3 location = GetLocation();
	inOutputStream.Write(location.x);
	inOutputStream.Write(location.y);

	inOutputStream.Write(GetColor());
	return 0;
}
