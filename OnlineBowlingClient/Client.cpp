#include "OnlineBowlingClientPCH.h"

// --------------------------------------------------------
string mPlayerName;
string mErrorMsg;
bool enterQueue = false;
enum class EPlayerNameValidationError
{
	EPNVE_EMPTY,
	EPNVE_MAX_LENGTH_EXCEEDED,

	EPNVE_NO_SPACES_ALLOWED,
	EPNVE_NO_SPECIAL_CHARACTERS_ALLOWED,

	EPNVE_NO_ERROR
};
EPlayerNameValidationError validate_name(const std::string& inPlayerName)
{
	int special = 0, l = inPlayerName.length();

	if (l == 0)
		return EPlayerNameValidationError::EPNVE_EMPTY;
	if (l > 16)
		return EPlayerNameValidationError::EPNVE_MAX_LENGTH_EXCEEDED;

	for (int i = 0; i < l; i++)
	{
		char s = inPlayerName.at(i);

		if (s == ' ')
			return EPlayerNameValidationError::EPNVE_NO_SPACES_ALLOWED;

		//characters other than alphabets and numbers
		if (isalnum(s))
			continue;
		else
		{
			return EPlayerNameValidationError::EPNVE_NO_SPECIAL_CHARACTERS_ALLOWED;
		}
	}
	return EPlayerNameValidationError::EPNVE_NO_ERROR;
}

void enter_game(const std::string& inPlayerName)
{
	enterQueue = true;
}

void validate_name_and_enter_game()
{
	auto error = validate_name(mPlayerName);
	switch (error)
	{
	case EPlayerNameValidationError::EPNVE_EMPTY:
		mErrorMsg = "Cannot be empty";
		break;
	case EPlayerNameValidationError::EPNVE_MAX_LENGTH_EXCEEDED:
		mErrorMsg = "Maximum number of characters exceeded";
		break;
	case EPlayerNameValidationError::EPNVE_NO_SPACES_ALLOWED:
		mErrorMsg = "No spaces are allowed";
		break;
	case EPlayerNameValidationError::EPNVE_NO_SPECIAL_CHARACTERS_ALLOWED:
		mErrorMsg = "No special characters are allowed";
		break;
	case EPlayerNameValidationError::EPNVE_NO_ERROR:
		mErrorMsg = "";
		enter_game(mPlayerName);
		break;
	default:
		break;
	}
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	mPlayerName += static_cast<char>(codepoint);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
	{
		if (mPlayerName.size() > 0)
		{
			mPlayerName.pop_back();
		}
	}
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		validate_name_and_enter_game();
	}
}
// --------------------------------------------------------

bool Client::StaticInit()
{
	sInstance.reset(new Client());
	return true;
}

Client::Client()
{
	GameObjectRegistry::sInstance->RegisterCreationFunction('BALL', BallClient::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('FLOO', FloorClient::StaticCreate);

	SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString("127.0.0.1:45000");
	NetworkManagerClient::StaticInit(*serverAddress, "");

	RenderManager::StaticInit();
	TextRenderer::StaticInit();

	InputManager::StaticInit();

	mState = Client::EClientState::ECS_Lobby;
	string name = StringUtils::GetCommandLineArg(1);
	if (name != "")
	{
		mState = Client::EClientState::ECS_Queuing;
		mPlayerName = name;
		GLFWwindow* window = RenderManager::sInstance->GetMainWindow();
		glfwSetCharCallback(window, NULL);
		glfwSetKeyCallback(window, NULL);
		NetworkManagerClient::sInstance->SetName(mPlayerName);
	}

	glfwSetCharCallback(RenderManager::sInstance->GetMainWindow(), character_callback);
	glfwSetKeyCallback(RenderManager::sInstance->GetMainWindow(), key_callback);
}

void Client::HandleInput()
{
	if (RenderManager::sInstance->GetWindowShouldClose())
	{
		SetShouldKeepRunning(false);
	}

	//glfwPollEvents();

	GLFWwindow* window = RenderManager::sInstance->GetMainWindow();
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
	TinyLogger log("Client::DoFrame", true);
	GLFWwindow* window = RenderManager::sInstance->GetMainWindow();
	static int count = 0;
	static float timeNextTick = Timing::sInstance.GetTimef();
	SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString("127.0.0.1:45000");

	float ScoreBoardOriginX = 25.0f;
	float ScoreBoardOriginY = SCR_HEIGHT * 0.9;
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		SetShouldKeepRunning(false);
	switch (mState)
	{
	case Client::EClientState::ECS_Lobby:
		TextRenderer::sInstance->RenderText("Enter your name", 25.0f, SCR_HEIGHT * 0.6f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		TextRenderer::sInstance->RenderText(mPlayerName.c_str(), 25.0f, SCR_HEIGHT * 0.4f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		if (mErrorMsg.size() > 0)
			TextRenderer::sInstance->RenderText(mErrorMsg.c_str(), 25.0f, SCR_HEIGHT * 0.3f, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));

		if (enterQueue)
		{
			mState = EClientState::ECS_Queuing;
			glfwSetCharCallback(window, NULL);
			glfwSetKeyCallback(window, NULL);
			NetworkManagerClient::sInstance->SetName(mPlayerName);
		}
		break;

	case Client::EClientState::ECS_Queuing:
		if (Timing::sInstance.GetTimef() > timeNextTick)
		{
			count++;
			timeNextTick = Timing::sInstance.GetTimef() + 0.5f;
		}
		switch (count % 3)
		{
		case 0:
			TextRenderer::sInstance->RenderText("Looking for other player.", 0.f, 0.f, 1.0f, glm::vec3(1.0f), ETextAlignment::ETA_Center_Both);
			break;
		case 1:
			TextRenderer::sInstance->RenderText("Looking for other player..", 0.f, 0.f, 1.0f, glm::vec3(1.0f), ETextAlignment::ETA_Center_Both);
			break;
		case 2:
			TextRenderer::sInstance->RenderText("Looking for other player...", 0.f, 0.f, 1.0f, glm::vec3(1.0f), ETextAlignment::ETA_Center_Both);
			break;
		default:
			break;
		}

		TextRenderer::sInstance->RenderText(mPlayerName.c_str(), 0.f, SCR_HEIGHT * 0.7f, 1.0f, glm::vec3(1.0f), ETextAlignment::ETA_Center_Vertical);
		NetworkManagerClient::sInstance->ProcessIncomingPackets();
		NetworkManagerClient::sInstance->SendOutgoingPackets();

		if (NetworkManagerClient::sInstance->GetState() == NetworkManagerClient::NetworkClientState::NCS_Replicating)
		{
			mState = Client::EClientState::ECS_In_Game;
		}
		break;

	case Client::EClientState::ECS_In_Game:
		HandleInput();
		InputManager::sInstance->Update();
		Engine::DoFrame(); // update all game objects' state
		NetworkManagerClient::sInstance->ProcessIncomingPackets();

		for (const auto& entry : ScoreBoardManager::sInstance->GetEntries())
		{
			TextRenderer::sInstance->RenderText(
				entry.GetFormattedNameScore(),
				ScoreBoardOriginX,
				ScoreBoardOriginY,
				1.0, entry.GetColor());
			ScoreBoardOriginY -= SCR_HEIGHT * 0.1;
		}

		RenderManager::sInstance->Render();
		NetworkManagerClient::sInstance->SendOutgoingPackets();
		break;
	default:
		break;
	}
	glfwSwapBuffers(window);
	glfwPollEvents();
}