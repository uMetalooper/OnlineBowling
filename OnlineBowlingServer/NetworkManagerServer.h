class NetworkManagerServer
{
public:
	static NetworkManagerServer* sInstance;

	static bool StaticInit(uint16_t inPort);
private:
	UDPSocketPtr mSocket;
};

