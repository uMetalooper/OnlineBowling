class Engine
{
public:

	virtual ~Engine();
	static std::unique_ptr< Engine >	sInstance;

	virtual void	Run();
	void			SetShouldKeepRunning(bool inShouldKeepRunning) { mShouldKeepRunning = inShouldKeepRunning; }

protected:

	Engine();

	virtual void	DoFrame();

private:

	bool	mShouldKeepRunning;

	virtual void DoRunLoop();

};
