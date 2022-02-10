class Server : public Engine
{
public:

	static bool StaticInit();

	virtual void DoFrame() override;

	virtual void DoRunLoop() override;

	void HandleNewClient(ClientProxyPtr inClientProxy);
	void HandleLostClient(ClientProxyPtr inClientProxy);

	BallPtr	GetBallForPlayer(int inPlayerId);
	void	SpawnBallForPlayer(int inPlayerId);

private:
	Server();

	bool	InitNetworkManager();
	void	SetupWorld();

};