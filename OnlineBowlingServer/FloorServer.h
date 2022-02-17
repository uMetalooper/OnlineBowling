class FloorServer: public Floor
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new FloorServer()); }
	void HandleDying() override;

	virtual bool HandleCollisionWithBall(Ball* inBall) override;
protected:
	FloorServer();

};