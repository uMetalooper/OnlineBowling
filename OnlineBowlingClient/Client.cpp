#include "OnlineBowlingClientPCH.h"

bool Client::StaticInit()
{
	Client* client = new Client();

	if (WindowManager::StaticInit() == false)
	{
		return false;
	}

	if (GraphicsDriver::StaticInit(WindowManager::sInstance->GetMainWindow()) == false)
	{
		return false;
	}

	RenderManager::StaticInit();
	InputManager::StaticInit();

	sInstance.reset(client);
	return true;
}

Client::Client()
{
	GameObjectRegistry::sInstance->RegisterCreationFunction('BALL', BallClient::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('FLOO', FloorClient::StaticCreate);

	string destination = StringUtils::GetCommandLineArg(1);
	string name = StringUtils::GetCommandLineArg(2);

	SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString(destination);

	NetworkManagerClient::StaticInit(*serverAddress, name);
}

void Client::DoRunLoop()
{
	if (WindowManager::sInstance->GetWindowShouldClose())
	{
		SetShouldKeepRunning(false);
	}

	glfwPollEvents();

	GLFWwindow* window = WindowManager::sInstance->GetMainWindow();
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		InputManager::sInstance->HandleInput(EIA_Pressed, 'a');
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
	{
		InputManager::sInstance->HandleInput(EIA_Released, 'a');
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		InputManager::sInstance->HandleInput(EIA_Pressed, 'd');
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
	{
		InputManager::sInstance->HandleInput(EIA_Released, 'd');
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		InputManager::sInstance->HandleInput(EIA_Pressed, 'k');
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE)
	{
		InputManager::sInstance->HandleInput(EIA_Released, 'k');
	}
}

void Client::DoFrame()
{
	InputManager::sInstance->Update();

	Engine::DoFrame(); // update all game objects' state

	NetworkManagerClient::sInstance->ProcessIncomingPackets();

	RenderManager::sInstance->Render();

	NetworkManagerClient::sInstance->SendOutgoingPackets();
}