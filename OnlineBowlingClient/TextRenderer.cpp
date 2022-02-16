#include "OnlineBowlingClientPCH.h"

std::unique_ptr<TextRenderer> TextRenderer::sInstance;

int TextRenderer::StaticInit()
{
	sInstance.reset(new TextRenderer());
	return 0;
}

TextRenderer::TextRenderer()
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// FreeType
	// --------
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	FT_Init_FreeType(&ft);

	std::string font_name = ResourcesManager::getAbsolutePathOf("Fonts/arial.ttf");
	FT_Face face;
	FT_New_Face(ft, font_name.c_str(), 0, &face);

	// set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// load first 128 characters of ASCII set
	for (unsigned char c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<unsigned int>(face->glyph->advance.x)
		};
		mCharacters.insert(std::pair<char, Character>(c, character));

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	string vertexShaderSource = ResourcesManager::getAbsolutePathOf(R"(Shaders\text.vs)");
	string fragmentShaderSource = ResourcesManager::getAbsolutePathOf(R"(Shaders\text.fs)");
	mShader = Shader(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
	mShader.use();
	mShader.setMat4("projection", projection);

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextRenderer::RenderText(std::string text, float x, float y, float scale, glm::vec3 color, ETextAlignment alignment)
{
	// activate corresponding render state	
	mShader.use();
	mShader.setVec3("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(mVAO);
	float xoff, yoff;
	float text_width, text_height;
	CalculateTextSize(text, scale, text_width, text_height);

	switch (alignment)
	{
	case ETextAlignment::ETA_No_Alignment:
		xoff = x;
		yoff = y;
		break;
	case ETextAlignment::ETA_Center_Horizontal:
		xoff = x;
		yoff = (SCR_HEIGHT - text_height) / 2.f;
		break;
	case ETextAlignment::ETA_Center_Vertical:
		xoff = (SCR_WIDTH - text_width) / 2.f;
		yoff = y;
		break;
	case ETextAlignment::ETA_Center_Both:
		xoff = (SCR_WIDTH - text_width) / 2.f;
		yoff = (SCR_HEIGHT - text_height) / 2.f;
		break;
	default:
		break;
	}
	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = mCharacters[*c];

		float xpos = xoff + ch.Bearing.x * scale;
		float ypos = yoff - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		xoff += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::CalculateTextSize(const std::string& inText, float scale, float& outTextWidth, float& outTextHeight)
{
	std::string::const_iterator c;
	float text_width = 0.f;
	float text_height = 0.f;
	for (c = inText.begin(); c != inText.end(); c++)
	{
		Character ch = mCharacters[*c];
		float w = ch.Size.x * scale; text_width += (ch.Advance >> 6) * scale;
		float h = ch.Size.y * scale; text_height = h > text_height ? h : text_height;
	}

	outTextWidth = text_width;
	outTextHeight = text_height;
}
