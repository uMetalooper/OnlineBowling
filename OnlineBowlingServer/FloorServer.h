class FloorServer: public Floor
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new FloorServer()); }
	void HandleDying() override;

protected:
	FloorServer();

};