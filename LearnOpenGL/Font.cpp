#include "Font.h"

Font::Font()
{
	InitFreeTypeLib();
}

Font::~Font()
{
}

int Font::InitFreeTypeLib()
{
	std::cout << "=========================================\n";
	if (FT_Init_FreeType(&_fLib))
	{
		std::cout << "ERROR::FREETYPE : Could not init FreeType Library\n";
		return -1;
	}
	else
	{
		std::cout << "LOG::FREETYPE : FreeType Library Initialized successfully\n";
	}

	if (FT_New_Face(_fLib, "Resources/Fonts/arial.ttf", 0, &_fFace))
	{
		std::cout << "ERROR::FREETYPE : Failed to load font\n";
		return -1;
	}
	else
	{
		std::cout << "LOG::FREETYPE : Fonts Loaded!\n";
	}

	FT_Set_Pixel_Sizes(_fFace, 0, 48);

	return 0;
}

void Font::GenerateAndStoreCharacters(int n)
{
	// disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < n; c++)
	{
		if (FT_Load_Char(_fFace, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE : Failed to load Glyph" << std::endl;
			continue;
		}

		_charTex.CreateFontTexture(_fFace->glyph->bitmap.width,
							_fFace->glyph->bitmap.rows,
							GL_CLAMP_TO_EDGE,
							GL_CLAMP_TO_EDGE,
							GL_LINEAR,
							GL_LINEAR,
							_fFace->glyph->bitmap.buffer);

		Character character =
		{
			_charTex.ID,																// CharID
			glm::ivec2(_fFace->glyph->bitmap.width, _fFace->glyph->bitmap.rows),	// Size
			glm::ivec2(_fFace->glyph->bitmap_left, _fFace->glyph->bitmap_top),		// Bearing
			_fFace->glyph->advance.x												// Advance
		};

		_characters.insert(std::pair<char, Character>(c, character));
	}

	FT_Done_Face(_fFace);
	FT_Done_FreeType(_fLib);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Font::CreateFontVertexArrayObject()
{
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Font::Draw(std::string text, float x, float y, float scale, glm::vec3 color, Camera *cam)
{
	glm::mat4 projection = cam->SetProjection(CameraType::ORTHGRAPHIC, 800, 600, 0, 0);
	
	glBindVertexArray(_VAO);

	// Shader
	Shader _fontShader("./Shaders/VSFont.vert", "./Shaders/FSFont.frag");

	_fontShader.Use();
	_fontShader.SetUniformVec3f("textColor", color);

	_charTex.Activate();


	// iterate through all characters
	std::string::const_iterator c;

	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = _characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

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
		glBindTexture(GL_TEXTURE_2D, ch.charID);

		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)

		std::cout << "\nRendering..." << *c << '\n';
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
