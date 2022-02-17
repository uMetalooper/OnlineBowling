class BallServer : public Ball
{
public:
	static uint8_t sRemovedIndex;
	static uint8_t sLastRemovedIndex;

	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new BallServer()); }
	void HandleDying() override;
	
	virtual void Update() override;

protected:
	BallServer();

	bool CheckOutOfBound();

};