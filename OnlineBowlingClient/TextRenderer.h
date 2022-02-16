enum class ETextAlignment
{
	ETA_No_Alignment = 0,
	ETA_Center_Horizontal = 1 << 0,
	ETA_Center_Vertical = 1 << 1,
	ETA_Center_Both = ETA_Center_Horizontal | ETA_Center_Vertical
};

class TextRenderer
{
	struct Character {
		unsigned int TextureID; // ID handle of the glyph texture
		glm::ivec2   Size;      // Size of glyph
		glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
		unsigned int Advance;   // Horizontal offset to advance to next glyph
	};

public:
	TextRenderer();

	static std::unique_ptr<TextRenderer> sInstance;

	static int StaticInit();

	void RenderText(std::string text, float x, float y, float scale, glm::vec3 color, ETextAlignment alignment = ETextAlignment::ETA_No_Alignment);

private:
	std::map<GLchar, Character> mCharacters;

	Shader mShader;
	unsigned int mVAO, mVBO;

	void CalculateTextSize(const std::string& inText, float scale, float& outTextWidth, float& outTextHeight);
};
