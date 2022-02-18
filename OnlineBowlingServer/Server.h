const float kGameGap = 1.2f;

class Server : public Engine
{
public:
	class Game
	{
	public:
		Game(int inGameId, BallPtr inBallA, BallPtr inBallB, BallPtr inPins[], FloorPtr inFloor)
			: mBallThrown(false)
		{
			mGameId = inGameId;
			mGameOffsetX = (inGameId - 1) * kGameGap;
			mBallA = inBallA;
			mBallB = inBallB;
			for (int i = 0; i < 10; i++)
			{
				mPins[i] = inPins[i];
			}
			mFloor = inFloor;
			mFloor->SetLocation(Vector3(mGameOffsetX, ALLEY_LENGTH / 2.0f, 0.0f));

			mPlayerIndex = 0;
			mPoint = 0;
			ResetBall();
			ResetPins();

		}
		void Update();

		int mGameId;
		float mGameOffsetX;
		BallPtr mActiveBall;
		BallPtr mBallA;
		BallPtr mBallB;
		FloorPtr mFloor;
		BallPtr mPins[10];

		bool mBallThrown;
		bool mTouchedIndex[10];
		int mRemovedIndex;

		int mPlayerIndex;
		int mPoint;

		void ResetPins();
		void ResetBall();

		bool CheckCollisions(BallPtr one, BallPtr two);
		void CollisionResponse(BallPtr one, BallPtr two);

		bool CheckBound(BallPtr ball);
	};
	typedef shared_ptr<Game> GamePtr;

public:
	static bool StaticInit();

	virtual void DoFrame() override;

	void HandleNewClient(ClientProxyPtr inClientProxy);
	void HandleNewGame(ClientProxyPtr inClientAProxy, ClientProxyPtr inClientBProxy);
	void HandleLostClient(ClientProxyPtr inClientProxy);

	BallPtr	GetBallForPlayer(int inPlayerId);
	void	SpawnBallForPlayer(int inPlayerId);
	void SpawnPinsForNewGame();

	int GetNewGameId() { return ++mGameCount; }

private:
	Server();

	bool	InitNetworkManager();


	std::map<int, GamePtr> mIdToGames;
	int mGameCount;
};