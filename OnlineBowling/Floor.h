class Floor : public GameObject
{
public:
	CLASS_IDENTIFICATION('FLOO', GameObject)

	enum EFloorReplicationState
	{
		EFRS_Pose = 1 << 0,
		EFRS_Color = 1 << 1,

		EFRS_AllState = EFRS_Pose | EFRS_Color
	};

	Floor()
	{
		
	}

	static	GameObject* StaticCreate() { return new Floor(); }
	virtual uint32_t	GetAllStateMask()	const override { return EFRS_AllState; }

	void			SetSize(const Vector3& inSize) { mSize = inSize; }
	const Vector3&  GetSize() const { return mSize; }

	virtual uint32_t	Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const override;

	virtual bool HandleCollisionWithBall(Ball* inBall) override;

private:
	Vector3 mSize;
};

typedef shared_ptr< Floor >	FloorPtr;