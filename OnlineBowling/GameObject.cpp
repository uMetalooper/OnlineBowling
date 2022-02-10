#include "OnlineBowlingPCH.h"


GameObject::GameObject() :
	mIndexInWorld(-1),
	mDoesWantToDie(false),
	mNetworkId(0),
	mColor(Vector3(1.0f))
{
}

void GameObject::PrintSelf()
{
	printf("Location: %f, %f, %f\n", mLocation.x, mLocation.y, mLocation.z);

}