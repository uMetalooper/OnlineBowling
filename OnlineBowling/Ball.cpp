#include "OnlineBowlingPCH.h"

const float fricCoeff = 0.03f;
const float g = 9.8f;

const float ALLEY_WIDTH = 1.0f; // 1 meter wide
const float HALF_ALLEY_WIDTH = 0.5f;
const float ALLEY_LENGTH = 18.288f; // 18.288 meters long

const float BALL_RADIUS = 0.1;

void Ball::Update()
{
	float deltaTime = Timing::sInstance.GetDeltaTime();
	Vector3 velocity = GetVelocity();
	if (glm::length(velocity) <= 0.000001f) return;
	// apply friction
	Vector3 acceleration = -glm::normalize(velocity);
	acceleration = acceleration * fricCoeff * g;
	Vector3 velocityChange = acceleration * deltaTime;
	if (glm::length(velocityChange) > glm::length(velocity))
	{
		SetVelocity(Vector3(0.0f));
	}
	else
	{
		SetVelocity(velocity + velocityChange);
	}

	// update position
	SetLocation(GetLocation() + GetVelocity() * deltaTime);
}

void Ball::ProcessInput(float inDeltaTime, const InputState& inInputState)
{
	//moving...
	float inputDelta = inInputState.GetDesiredHorizontalDelta();
	Vector3 oldLocation = GetLocation();
	Vector3 newLocation = oldLocation + Vector3(inputDelta * inDeltaTime, 0.0f, 0.0f);
	if (newLocation.x < -HALF_ALLEY_WIDTH)
	{
		SetLocation(Vector3(-HALF_ALLEY_WIDTH, newLocation.y, newLocation.z));
	}
	else if (newLocation.x > HALF_ALLEY_WIDTH)
	{
		SetLocation(Vector3(HALF_ALLEY_WIDTH, newLocation.y, newLocation.z));
	}
	else
	{
		SetLocation(newLocation);
	}

	mIsShooting = inInputState.IsShooting();
}

uint32_t Ball::Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const
{
	uint32_t writtenState = 0;
	if (inDirtyState | EBRS_PlayerId)
	{
		inOutputStream.Write((bool)true);
		inOutputStream.Write(GetPlayerId());
		writtenState |= EBRS_PlayerId;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	if (inDirtyState | EBRS_Pose)
	{
		inOutputStream.Write((bool)true);
		Vector3 velocity = mVelocity;
		inOutputStream.Write(velocity.x);
		inOutputStream.Write(velocity.y);

		Vector3 location = GetLocation();
		inOutputStream.Write(location.x);
		inOutputStream.Write(location.y);

		writtenState |= EBRS_Pose;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}
	
	if (inDirtyState | EBRS_Color)
	{
		inOutputStream.Write((bool)true);
		inOutputStream.Write(GetColor());
		writtenState |= EBRS_Color;
	}
	else
	{
		inOutputStream.Write((bool)false);
	}

	return writtenState;
}
