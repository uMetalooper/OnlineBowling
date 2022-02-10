class Client : public Engine
{
public:

	static bool StaticInit();

protected:

	Client();

	virtual void	DoRunLoop() override;
	virtual void	DoFrame() override;

private:
};