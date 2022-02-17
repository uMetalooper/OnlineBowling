class Ball : public GameObject
{
public:
	CLASS_IDENTIFICATION('BALL', GameObject)

	enum EBallReplicationState
	{
		EBRS_Pose = 1 << 0,
		EBRS_Color = 1 << 1,
		EBRS_PlayerId = 1 << 2,

		EBRS_AllState = EBRS_Pose | EBRS_Color | EBRS_PlayerId
	};

	Ball() : GameObject()
	{
		mActive = true;
		mIsTouched = false;
	}

	virtual	Ball* GetAsBall() override { return this; }

	static	GameObject* StaticCreate() { return new Ball(); }

	virtual uint32_t GetAllStateMask()	const override { return EBRS_AllState; }

	virtual void Update() override;

	void SetActive(bool s) { mActive = s; }
	bool GetActive() { return mActive; }

	bool IsStop() { return glm::length(mVelocity) < 0.000001f; }

	void ProcessInput(float inDeltaTime, const InputState& inInputState);

	void		SetPlayerId(uint32_t inPlayerId) { mPlayerId = inPlayerId; }
	uint32_t	GetPlayerId()						const { return mPlayerId; }

	void			SetVelocity(const Vector3& inVelocity) { mVelocity = inVelocity; }
	const Vector3&  GetVelocity()						const { return mVelocity; }

	virtual uint32_t	Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const override;

	void SetIsTouched(bool inIsTouched) { mIsTouched = inIsTouched; }
	bool GetIsTouched() { return mIsTouched; }

private:
	Vector3				mVelocity;
	bool mActive;
	bool mIsTouched;

	uint32_t			mPlayerId;
protected:
	bool mIsShooting;
};

typedef shared_ptr< Ball >	BallPtr;