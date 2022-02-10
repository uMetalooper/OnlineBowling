#include "OnlineBowlingClientPCH.h"

int main(int argc, const char** argv)
{
	if (Client::StaticInit())
	{
		Client::sInstance->Run();
	}
	else
	{
		//SDL_Quit();
		//error
		return 1;
	}
}