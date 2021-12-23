#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glad/glad.h"
#include "stb_image.h"

class Texture
{
public:
	// Texture ID (Unique Identifier)
	uint32_t ID;

	// Identify the ith time created texture
	int textureNumber;

	// Constructor to accept 
	// 1. imgPath		: Image Path (string)
	// 2. wrapSType		: Wraping Mode around 's' axis (int/GLint)
	// 3. wrapTType		: Wraping Mode around 't' axis (int/GLint)
	// 4. filtMinType	: Texture filter type for 'minfying' (int/GLint)
	// 5. filtMagType	: Texture filter type for 'magnifying' (int/GLint)
	//
	// To use image as a texture
	Texture(const char* imgPath,
			GLint wrapSType,
			GLint wrapTType,
			GLint filtMinType,
			GLint filtMagType);


	// Constructor to accept 
	// 1. width			: Texture width
	// 2. height		: Texture height
	// 3. filtMinType	: Texture filter type for 'minfying' (int/GLint)
	// 4. filtMagType	: Texture filter type for 'magnifying' (int/GLint)
	//
	// To use texture as a color buffer for framebuffer
	Texture(GLuint width, GLuint height, GLint filtMinType,	GLint filtMagType);


	// Use/Activate the texture
	void Activate();

private:
	// Total number of textures generated
	static int _textureCount;

};
