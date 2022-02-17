class Server : public Engine
{
public:
	class Game
	{
	public:
		void Update();

		BallPtr mBallA;
		BallPtr mBallB;
		FloorPtr mFloor;
		BallPtr mPins[10];
	};
	typedef shared_ptr<Game> GamePtr;

public:
	static int sGameId;

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


	std::map<int, GamePtr> mIdToGames;
};