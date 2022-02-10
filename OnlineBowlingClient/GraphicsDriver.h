
class GraphicsDriver
{
public:

	static bool StaticInit(GLFWwindow* inWnd);

	static std::unique_ptr< GraphicsDriver >		sInstance;

	void					Clear();
	void					Present();

	~GraphicsDriver();

private:

	GraphicsDriver();
	bool Init(GLFWwindow* inWnd);

	GLFWwindow* mWindow;
};
