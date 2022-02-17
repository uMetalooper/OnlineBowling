#include "OnlineBowlingPCH.h"

std::unique_ptr< Engine >	Engine::sInstance;


Engine::Engine() :
	mShouldKeepRunning(true)
{
	SocketUtil::StaticInit();

	srand(static_cast<uint32_t>(time(nullptr)));

	GameObjectRegistry::StaticInit();


	World::StaticInit();

	ScoreBoardManager::StaticInit();

	//SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
}

Engine::~Engine()
{
	SocketUtil::CleanUp();

	//SDL_Quit();
}




void Engine::Run()
{
	DoRunLoop();
}

void Engine::DoRunLoop()
{
	while (mShouldKeepRunning)
	{
		Timing::sInstance.Update();
		DoFrame();
	}
}

void Engine::DoFrame()
{
	World::sInstance->Update();
}


