#include "Texture.h"

int Texture::_textureCount = 0;

Texture::Texture(const char* imgPath, GLint wrapSType, GLint wrapTType, GLint filtMinType, GLint filtMagType)
{
	glGenTextures(1, &ID);

	stbi_set_flip_vertically_on_load(true);

	// Loading an image into bytes for texturing
	int width, height, nrChannels;
	unsigned char* data = stbi_load(imgPath, &width, &height, &nrChannels, 0);


	if (data)
	{
		GLenum type;
		switch (nrChannels)
		{
			case 3:
				type = GL_RGB;
				break;
			case 4:
				type = GL_RGBA;
				break;
			default:
				type = GL_RED;
				break;
		}

		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapSType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapTType);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtMinType);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtMagType);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	
	stbi_image_free(data);

	textureNumber = _textureCount;
	_textureCount++;

	std::cout << "LOG::TEXTURE::NUMBER : " << textureNumber << std::endl;
	std::cout << "LOG::TEXTURE::CHANNELS : " << nrChannels << std::endl;
	std::cout << "LOG::TEXTURE::COUNT : " << _textureCount << std::endl;
	std::cout << "===================================" << std::endl;
}

Texture::Texture(GLuint width, GLuint height, GLint filtMinType, GLint filtMagType)
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtMinType);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtMagType);

	textureNumber = _textureCount;
	_textureCount++;

	std::cout << "LOG::TEXTURE::NUMBER : " << textureNumber << std::endl;
	std::cout << "LOG::TEXTURE::CHANNELS : " << 3 << std::endl;
	std::cout << "LOG::TEXTURE::COUNT : " << _textureCount << std::endl;
	std::cout << "===================================" << std::endl;
}

void Texture::Activate()
{
	glActiveTexture(GL_TEXTURE0 + textureNumber);
	glBindTexture(GL_TEXTURE_2D, ID);
}