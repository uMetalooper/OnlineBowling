class WindowManager
{
	//asd
public:

	static bool StaticInit();
	static std::unique_ptr< WindowManager >	sInstance;

	GLFWwindow* GetMainWindow()	const { return mMainWindow; }
	bool        GetWindowShouldClose() { return glfwWindowShouldClose(mMainWindow); }

	~WindowManager();
private:
	WindowManager(GLFWwindow* inMainWindow);

	GLFWwindow* mMainWindow;
};