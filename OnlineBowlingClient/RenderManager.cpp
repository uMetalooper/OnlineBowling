#include "OnlineBowlingClientPCH.h"

std::unique_ptr< RenderManager >	RenderManager::sInstance;

RenderManager::RenderManager()
{
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
	GraphicsDriver::sInstance->Clear();

	RenderManager::sInstance->RenderDrawables();

	GraphicsDriver::sInstance->Present();
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

