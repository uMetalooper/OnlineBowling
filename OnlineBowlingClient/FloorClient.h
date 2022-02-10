class FloorClient : public Floor
{
public:
	static GameObjectPtr StaticCreate() { return GameObjectPtr(new FloorClient()); }

	virtual void Update() {}

	virtual void	Read(InputMemoryBitStream& inInputStream) override;

protected:
	FloorClient()
	{
		renderer.reset(new Plane(this));
	}

private:
	shared_ptr<Plane> renderer;
};

