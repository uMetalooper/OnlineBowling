class Server : public Engine
{
public:

	static bool StaticInit();

	virtual void DoFrame() override;

	void HandleNewClient(ClientProxyPtr inClientProxy);
	void HandleNewGame(ClientProxyPtr inClientAProxy, ClientProxyPtr inClientBProxy);
	void HandleLostClient(ClientProxyPtr inClientProxy);

	BallPtr	GetBallForPlayer(int inPlayerId);
	void	SpawnBallForPlayer(int inPlayerId);
	void SpawnPinsForNewGame();

private:
	Server();

	bool	InitNetworkManager();

};