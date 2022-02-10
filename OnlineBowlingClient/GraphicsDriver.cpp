#include "OnlineBowlingClientPCH.h"


std::unique_ptr< GraphicsDriver >	GraphicsDriver::sInstance;

namespace
{
}


bool GraphicsDriver::StaticInit(GLFWwindow* inWnd)
{
	GraphicsDriver* newGraphicsDriver = new GraphicsDriver();
	bool result = newGraphicsDriver->Init(inWnd);

	if (!result)
	{
		delete newGraphicsDriver;
	}
	else
	{
		sInstance.reset(newGraphicsDriver);
	}

	return result;
}

bool GraphicsDriver::Init(GLFWwindow* inWnd)
{
	mWindow = inWnd;
	return true;
}

GraphicsDriver::GraphicsDriver()
	: mWindow(nullptr)
{
}


GraphicsDriver::~GraphicsDriver()
{
}

void GraphicsDriver::Clear()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicsDriver::Present()
{
	glfwSwapBuffers(mWindow);
}

