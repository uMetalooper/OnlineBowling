class Plane : public Drawable
{
public:
	Plane()
		: center(0.0f), size(1.0f), rotateX(0.0f), rotateY(0.0f), rotateZ(0.0f), color(0.0f)
	{
		BuildPlane();
	}

	Plane(GameObject* inGameObject)
		: center(0.0f), size(1.0f), rotateX(0.0f), rotateY(0.0f), rotateZ(0.0f), color(0.0f), Drawable(inGameObject)
	{
		BuildPlane();
	}

	void setCenter(glm::vec3 newCenter)
	{
		center = newCenter;
	}

	void setSize(glm::vec2 newSize)
	{
		size = newSize;
	}

	void setRotate(glm::vec3 newRotate)
	{
		rotateX = newRotate.x;
		rotateY = newRotate.y;
		rotateZ = newRotate.z;
	}

	void setColor(glm::vec3 newColor)
	{
		color = newColor;
	}

	void Draw(Shader& shader);

private:
	glm::vec3 center;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 front;
	float rotateX;
	float rotateY;
	float rotateZ;
	glm::vec2 size;

	glm::vec3 color;

	void BuildPlane();
};

