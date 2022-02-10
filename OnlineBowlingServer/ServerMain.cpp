#include "OnlineBowlingServerPCH.h"
int main(int argc, const char** argv)
{
	if (Server::StaticInit())
	{
		Server::sInstance->Run();
	}
	return 0;
}