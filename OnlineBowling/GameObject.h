#define CLASS_IDENTIFICATION( inCode, inClass ) \
enum { kClassId = inCode }; \
virtual uint32_t GetClassId() const { return kClassId; } \
static GameObject* CreateInstance() { return static_cast< GameObject* >( new inClass() ); } \

class GameObject
{
public:

	CLASS_IDENTIFICATION('GOBJ', GameObject)

	GameObject();
	virtual ~GameObject() {}

	virtual	Ball* GetAsBall() { return nullptr; }

	virtual uint32_t GetAllStateMask()	const { return 0; }

	//return whether to keep processing collision
	virtual bool	HandleCollisionWithBall(Ball* inBall) { (void)inBall; return true; }

	virtual void	Update() {}
	virtual void	HandleDying() {}

	void	SetIndexInWorld(int inIndex) { mIndexInWorld = inIndex; }
	int		GetIndexInWorld()				const { return mIndexInWorld; }

	const Vector3&  GetLocation()				const { return mLocation; }
		void		SetLocation(const Vector3& inLocation) { mLocation = inLocation; }

		float		GetCollisionRadius()		const { return mCollisionRadius; }
		void		SetCollisionRadius(float inRadius) { mCollisionRadius = inRadius; }

	const Vector3&  GetColor()					const { return mColor; }
		void		SetColor(const Vector3& inColor) { mColor = inColor; }


	int			GetNetworkId()				const { return mNetworkId; }
	void		SetNetworkId(int inNetworkId) { mNetworkId = inNetworkId; }

	bool		DoesWantToDie()				const { return mDoesWantToDie; }
	void		SetDoesWantToDie(bool inWants) { mDoesWantToDie = inWants; }
	
	virtual uint32_t	Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const { (void)inOutputStream; (void)inDirtyState; return 0; }
	virtual void		Read(InputMemoryBitStream& inInputStream) { (void)inInputStream; }

	void PrintSelf();

private:

	Vector3 mLocation;
	Vector3 mColor;

	float											mCollisionRadius;

	int mIndexInWorld;

	bool mDoesWantToDie;

	int mNetworkId;
};

typedef shared_ptr< GameObject >	GameObjectPtr;