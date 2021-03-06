class RenderManager
{
public:

	static void StaticInit();
	static std::unique_ptr< RenderManager >	sInstance;

	void Render();

	void AddDrawable(Drawable* inDrawable);
	void RemoveDrawable(Drawable* inDrawable);
	int GetDrawableIndex(Drawable* inDrawable);

	void RenderDrawables();

	GLFWwindow* GetMainWindow() { return mMainWindow; }
	bool GetWindowShouldClose() { return glfwWindowShouldClose(mMainWindow); }

private:
	RenderManager();

	GLFWwindow* mMainWindow;

	Shader mShader;
	vector<Drawable*> mDrawables;
};

