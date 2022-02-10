class Sphere : public Drawable
{
public:
	Sphere() {}

	Sphere(GameObject* inGameObject) 
		: color(0.0f), Drawable(inGameObject)
	{
		buildVertices();
		initRenderData();
	}

	~Sphere()
	{
	}

	// legacy code
	void setPosition(glm::vec3 pos) { position = pos; }
	void setColor(glm::vec3 c) { color = c; }
	void setRadius(float r) { radius = r; }

	void Draw(Shader& shader);

private:
	void buildVertices();

	glm::vec3 position;
	glm::vec3 color;

	float radius;
};

