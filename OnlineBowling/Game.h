class Game
{
public:
	Game();
	Game(int inGameId);

	void applyImpulse(glm::vec2 v)
	{
		ball.SetVelocity(Vector3(v, 0.0f));
		ballThrown = true;
	}

	unsigned int getPlayerIndex() { return playerIndex; }

	void Update(float dt);

	//void Render();

private:
	int mGameId;
	float mGameOffsetX;
	//Camera mainCamera;

	Ball ball;
	bool ballThrown;

	Ball pins[10];
	bool touchedIndex[10];
	int removedIndex;

	Floor floor;

	//Shader shader;

	void ResetBall();
	void ResetPins();

	bool CheckCollisions(Ball& one, Ball& two);
	void ApplyCollision(Ball& one, Ball& two);

	bool CheckBound(Ball b);

	/*void setCamera(glm::mat4 view)
	{
		shader.setMat4("view", view);
	}*/

	// gameplay
	unsigned int playerIndex;
	float timer;
};

