#pragma once

#include <iostream>
#include <map>

#include "glm/glm.hpp"
#include "glm/ext/matrix_clip_space.hpp"

#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "ft2build.h"
#include FT_FREETYPE_H

struct Character
{
	unsigned int charID;	// Id of each character
	glm::ivec2 Size;		// Size of glyph
	glm::ivec2 Bearing;		// Offset from baseline to left/top of glyph
	FT_Pos Advance;	// Offset to advance to next glyph
};

class Font
{
private: // Variables
	
	FT_Library _fLib;
	FT_Face _fFace;

	// Mapped actual characters with fonts
	std::map<char, Character> _characters;

	// Generate texture
	Texture _charTex;

	unsigned int _VAO, _VBO;

public:
	Font();
	~Font();

private:
	int InitFreeTypeLib();

public:
	void GenerateAndStoreCharacters(int);
	void CreateFontVertexArrayObject();
	void Draw(std::string text, float x, float y, float scale, glm::vec3 color, Camera *cam);
};

