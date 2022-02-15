class Drawable
{
public:
	Drawable() :
		mGameObject(nullptr)
	{

	}

	Drawable(GameObject* inGameObject);
	~Drawable();

	GameObject* GetGameObject() { return mGameObject; }

	void addVertex(float x, float y, float z);
	void addNormal(float nx, float ny, float nz);
	void addTexCoord(float s, float t);

	void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);

	unsigned int getVertexCount() const { return (unsigned int)vertices.size() / 3; }
	unsigned int getInterleavedVertexSize() const { return (unsigned int)interleavedVertices.size() * sizeof(float); }
	unsigned int getIndexSize() const { return (unsigned int)indices.size() * sizeof(unsigned int); }

	void buildInterleavedVertices();
	void initRenderData();

	virtual void Draw(Shader& shader) = 0;

protected:
	GameObject* mGameObject;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;
	std::vector<unsigned int> indices;

	std::vector<float> interleavedVertices;

	unsigned int VAO;
};

