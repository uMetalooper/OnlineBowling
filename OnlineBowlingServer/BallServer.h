class BallServer : public Ball
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new BallServer()); }
	void HandleDying() override;
	
	virtual void Update() override;

protected:
	BallServer();

};