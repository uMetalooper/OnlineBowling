class BallClient : public Ball
{
public:
	static GameObjectPtr StaticCreate() { return GameObjectPtr(new BallClient()); }

	virtual void Update() {}

	virtual void	Read(InputMemoryBitStream& inInputStream) override;

protected:
	BallClient()
	{
		renderer.reset(new Sphere(this));
	}

private:
	shared_ptr<Sphere> renderer;
};

