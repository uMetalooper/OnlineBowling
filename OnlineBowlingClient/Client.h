class Client : public Engine
{
	enum EClientState
	{
		ECS_Lobby,
		ECS_Queuing,
		ECS_In_Game
	};
public:

	static bool StaticInit();

protected:

	Client();

	virtual void	DoFrame() override;
	
	void HandleInput();

private:
	EClientState mState;
};