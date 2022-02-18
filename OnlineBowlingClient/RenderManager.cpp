#include "OnlineBowlingClientPCH.h"

std::unique_ptr< RenderManager >	RenderManager::sInstance;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

RenderManager::RenderManager()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mMainWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Online Bowling Game", NULL, NULL);

	glfwMakeContextCurrent(mMainWindow);

	// GLAD
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glfwSetFramebufferSizeCallback(mMainWindow, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);

	string vertexShaderSource = ResourcesManager::getAbsolutePathOf(R"(Shaders\Sphere.vert)");
	string fragmentShaderSource = ResourcesManager::getAbsolutePathOf(R"(Shaders\Sphere.frag)");
	mShader = Shader(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

	mShader.use();
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
	mShader.setMat4("projection", proj);

	Vector3 camPos(0.0f, -3.0f, 2.0f);
	Vector3 target(0.0f, 1.0f, 0.0f);
	Vector3 up(0.0f, 0.0f, 1.0f);
	glm::mat4 view = glm::lookAt(camPos, target, up);
	mShader.setMat4("view", view);
}

void RenderManager::StaticInit()
{
	sInstance.reset(new RenderManager());
}

void RenderManager::Render()
{
	// TODO: pre-assign player ball to avoid looping thorugh every game objects every frame
	for (auto goIt = World::sInstance->GetGameObjects().begin(); goIt != World::sInstance->GetGameObjects().end(); goIt++)
	{
		Ball* ball = goIt->get()->GetAsBall();
		if (ball)
		{
			if (ball->GetPlayerId() == NetworkManagerClient::sInstance->GetPlayerId())
			{
				Vector3 targetPos = Vector3(ball->GetLocation().x, ball->GetLocation().y + 2.0f, 0.0f);  // in front of the ball 2m
				Vector3 cameraPos = Vector3(ball->GetLocation().x, ball->GetLocation().y - 2.0f, 2.0f);  // behind the ball 2m, above 2m
				Vector3 up = glm::vec3(0.0f, 0.0f, 1.0f);
				glm::mat4 view = glm::lookAt(cameraPos, targetPos, up);
				mShader.use();
				mShader.setMat4("view", view);
			}
		}
	}

	RenderManager::sInstance->RenderDrawables();
}

void RenderManager::AddDrawable(Drawable* inDrawable)
{
	mDrawables.push_back(inDrawable);
}

void RenderManager::RemoveDrawable(Drawable* inDrawable)
{
	int index = GetDrawableIndex(inDrawable);

	if (index != -1)
	{
		int lastIndex = mDrawables.size() - 1;
		if (index != lastIndex)
		{
			mDrawables[index] = mDrawables[lastIndex];
		}
		mDrawables.pop_back();
	}
}

int RenderManager::GetDrawableIndex(Drawable* inDrawable)
{
	for (int i = 0, c = mDrawables.size(); i < c; ++i)
	{
		if (mDrawables[i] == inDrawable)
		{
			return i;
		}
	}

	return -1;
}

void RenderManager::RenderDrawables()
{
	for (auto cIt = mDrawables.begin(), end = mDrawables.end(); cIt != end; ++cIt)
	{
		(*cIt)->Draw(mShader);
	}
}

