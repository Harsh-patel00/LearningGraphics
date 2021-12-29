#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "glad/glad.h"
#include "glfw3.h"
#include "glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "assimp/mesh.h"
#include "ft2build.h"
#include FT_FREETYPE_H

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

#define print std::cout << 
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#pragma region Texture related global var

float mixValue = 0.4f;

#pragma endregion

#pragma region Time/Frame related gloabl var

float deltaTime = 0.0f;
float lastFrame = 0.0f;
auto maxFPS = -INT16_MAX;
auto minFPS = INT16_MAX;

#pragma endregion

#pragma region Camera realted global var

// Define camera
Camera ourCamera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 
	WINDOW_WIDTH, WINDOW_HEIGHT);

#pragma endregion

#pragma region Light related global var

// glm::vec3 lightPos(2.0f, 0.0f, 0.0f); // Set light src on +ve x-axis

#pragma endregion

#pragma region Font related global var

struct Character {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};

std::map<char, Character> Characters;
unsigned int fontVAO, fontVBO;

#pragma endregion


// Function called when a window is resized
void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	std::cout << "Window resized to : (" << width << ", " << height << ")" << std::endl;
}

// Function for keeping inputs organized
void ProcessInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	const float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		ourCamera.ProcessKeyboard(CameraMoveDir::FORWARD, cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		ourCamera.ProcessKeyboard(CameraMoveDir::BACKWARD, cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		ourCamera.ProcessKeyboard(CameraMoveDir::FORWARD, cameraSpeed * 2.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		ourCamera.ProcessKeyboard(CameraMoveDir::BACKWARD, cameraSpeed * 2.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		ourCamera.ProcessKeyboard(CameraMoveDir::LEFT, cameraSpeed);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		ourCamera.ProcessKeyboard(CameraMoveDir::RIGHT, cameraSpeed);
	}
}

// Function called when mouse is moved
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	ourCamera.ProcessMouseMove((float)xPos, (float)yPos);
}

// Function called when scroll wheel is moved
void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	ourCamera.ProcessMouseScroll(window, xoffset, yoffset);
}

// render line of text
void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
	// activate corresponding render state	
	shader.Use();
	shader.SetUniformVec3f("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(fontVAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

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
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, fontVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int LoadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	stbi_set_flip_vertically_on_load(false);

	int width, height, nrChannels;
	
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

int main()
{
	#pragma region Initialization of GLFW and GLAD

	// Initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creating a window
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGLWindow", nullptr, nullptr);

	if (window == NULL)
	{
		std::cout << "Failed to create a GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initializing GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}

	// Display Window
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	// Set callbacks
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);		
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	// Enable depth buffer testing to properly draw vertices
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Lock the cursor at the center of the screen when application starts
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	std::cout << "============== BASIC INFO ===============" << std::endl;
	std::cout << "GLFW Version : " << glfwGetVersionString() << std::endl;
	std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "=========================================" << std::endl;

	#pragma endregion

	#pragma region Shaders

	// Shaders

	const char *simpleVert		= "./Shaders/SimpleVert.vert";
	const char *simpleFrag		= "./Shaders/SimpleFrag.frag";

	const char *simpleVS2		= "./Shaders/SimpleVert2.vert";
	const char *simpleFS2		= "./Shaders/SimpleFrag2.frag";
	
	const char *fontVS		= "./Shaders/VSFont.vert";
	const char *fontFS		= "./Shaders/FSFont.frag";
	
	const char *cubemapVS		= "./Shaders/VSCubemap.vert";
	const char * cubemapFS		= "./Shaders/FSCubemap.frag";

	Shader textureShader(simpleVert, simpleFrag);	// For object using textures
	Shader framebufferShader(simpleVS2, simpleFS2); // For framebuffer textures
	Shader cubemapShader(cubemapVS, cubemapFS);		// For cubemap

	Shader shader(fontVS, fontFS);					// For text

	glm::mat4 fontProjection = ourCamera.SetProjection(CameraType::ORTHGRAPHIC, static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT), 0, 0);
	shader.Use();
	shader.SetMatrixUniform4f("fontProjection", glm::value_ptr(fontProjection));

	#pragma endregion

	#pragma region Textures

	const char* tex1Source = "./Resources/container.jpg";
	const char* tex2Source = "./Resources/marble.jpg";

	Texture tex1(tex1Source, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR);
	Texture tex2(tex2Source, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR);

	std::vector<std::string> textures_faces
	{
		"./Resources/skybox/right.jpg",
		"./Resources/skybox/left.jpg",
		"./Resources/skybox/top.jpg",
		"./Resources/skybox/bottom.jpg",
		"./Resources/skybox/front.jpg",
		"./Resources/skybox/back.jpg",
	};

	unsigned int cubemapTexture = LoadCubemap(textures_faces);

	#pragma endregion

	#pragma region Fonts

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return -1;
	}

	FT_Face face;
	if (FT_New_Face(ft, "Resources/Fonts/arial.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return -1;
	}
	else
	{
		FT_Set_Pixel_Sizes(face, 0, 48);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

		for (unsigned char c = 0; c < 128; c++)
		{
			// load character glyph 
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
				face->glyph->advance.x
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	#pragma endregion

	#pragma region Vertices

	float cubeVertices[] = {
		// Back face
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		// Front face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		// Left face
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		// Right face
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
		// Bottom face
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		// Top face
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
	};
	
	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};

	float quadHalfVertices[] = {
		// positions   // texCoords
		-.5f,  .5f,  0.0f, 1.0f,
		-.5f, -.5f,  0.0f, 0.0f,
		 .5f, -.5f,  1.0f, 0.0f,

		-.5f,  .5f,  0.0f, 1.0f,
		 .5f, -.5f,  1.0f, 0.0f,
		 .5f,  .5f,  1.0f, 1.0f
	};
	
	float quadFullVertices[] = {
		// positions   // texCoords
		-1.f,  1.f,  0.0f, 1.0f,
		-1.f, -1.f,  0.0f, 0.0f,
		 1.f, -1.f,  1.0f, 0.0f,

		-1.f,  1.f,  0.0f, 1.0f,
		 1.f, -1.f,  1.0f, 0.0f,
		 1.f,  1.f,  1.0f, 1.0f
	};
	
	float quadMapVertices[] = {
		// positions   // texCoords
		-1.0f, -.25f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		-.25f, -1.0f,  1.0f, 0.0f,

		-1.0f, -.25f,  0.0f, 1.0f,
		-.25f, -1.0f,  1.0f, 0.0f,
		-.25f, -.25f,  1.0f, 1.0f
	};

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	#pragma endregion

	#pragma region VAOs & VBOs & FBOs

	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	// plane VAO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);


	// Quad for Framebuffer VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadMapVertices), &quadMapVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);

	// Framebuffer (FBO)
	
	unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		// Creating texture
		Texture texColorBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, GL_LINEAR, GL_LINEAR);

		// Framebuffer with textureColorBuffer as an attachment
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer.ID, 0);

		// Renderbuffer object (RBO)
		unsigned int RBO;
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);

		// Creating a render buffer storage to store depth and stencil values
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_WIDTH, WINDOW_HEIGHT);

		// Unbind renderbuffer as we have stored all the params
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Attaching our new renderbuffer object to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// Check if framebuffer have all the attachements
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		print "ERROR::FRAMEBUFFER::Framebuffer not complete!\n";
	}

	// Unbind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Quad for fonts
	glGenVertexArrays(1, &fontVAO);
	glGenBuffers(1, &fontVBO);
	glBindVertexArray(fontVAO);
	glBindBuffer(GL_ARRAY_BUFFER, fontVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	#pragma endregion

	// RENDER LOOP
	while (!glfwWindowShouldClose(window))
	{
		// Calulations for deltaTime at the start of each frame
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// float FPS = 1/deltaTime;
		// print FPS << "\n";

		// Process the input while the window is being displayed
		ProcessInput(window);

		// Set framebuffer on which we want to draw the scene
		// glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		// Rendering commands
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Use this color when clearing color buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer, z-buffer and stencil buffer

		int fps = (int)(1 / deltaTime);

		if (fps > maxFPS) { maxFPS = fps; }
		if (fps < minFPS) { minFPS = fps; }

		std::string fps_str = "FPS : " + std::to_string(fps);


		// To render the output in wireframe mode, uncomment the below line
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		// Set the projection to perspective (Need to set this every frame as we are zooming-in and -out)
		glm::mat4 projection = glm::mat4(1.0f);
		projection = ourCamera.SetProjection(CameraType::PERSPECTIVE, WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 100.0f);

		// Set the view matrix (Need to set this every frame as we are moving the camera around)
		glm::mat4 view;
		view = ourCamera.LookAt();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.f));

		#pragma region Cubes

		// 1st Cube
		glBindVertexArray(cubeVAO);
		
		textureShader.Use();

		tex1.Activate();

		textureShader.SetInt("texture1", tex1.textureNumber);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.f));
		textureShader.SetMatrixUniform4f("model", glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//// Second Cube
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(1.0f, 0.f, 1.5f));
		//textureShader.SetMatrixUniform4f("model", glm::value_ptr(model));

		//glDrawArrays(GL_TRIANGLES, 0, 36);

		textureShader.SetMatrixUniform4f("view", glm::value_ptr(view));
		textureShader.SetMatrixUniform4f("projection", glm::value_ptr(projection));

		#pragma endregion

		glBindVertexArray(0);

		#pragma region DefaultFramebuffer

		//
		//// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//glBindFramebuffer(GL_FRAMEBUFFER, 0); // Use default framebuffer
		//glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_DEPTH_TEST);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.f));

		//// Plane
		//glBindVertexArray(planeVAO);

		//textureShader.Use();

		//tex2.Activate();

		//textureShader.SetInt("texture1", tex2.textureNumber);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(-0.001f));

		//textureShader.SetMatrixUniform4f("model", glm::value_ptr(model));
		//textureShader.SetMatrixUniform4f("view", glm::value_ptr(view));
		//textureShader.SetMatrixUniform4f("projection", glm::value_ptr(projection));

		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//// Cube
		//glBindVertexArray(cubeVAO);

		//textureShader.Use();

		//tex1.Activate();

		//textureShader.SetInt("texture1", tex1.textureNumber);

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.f));
		//textureShader.SetMatrixUniform4f("model", glm::value_ptr(model));

		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//// Second Cube
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(1.0f, 0.f, 1.5f));
		//textureShader.SetMatrixUniform4f("model", glm::value_ptr(model));

		//glDrawArrays(GL_TRIANGLES, 0, 36);


		//framebufferShader.Use();

		//texColorBuffer.Activate();

		//framebufferShader.SetInt("screenTexture", texColorBuffer.textureNumber);

		//glBindVertexArray(quadVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		#pragma endregion

		#pragma region Cubemap/Skyox

		glDepthFunc(GL_LEQUAL);
		cubemapShader.Use();

		view = glm::mat4(1.0f);
		view = glm::mat4(glm::mat3(ourCamera.LookAt()));
		// ... set view and projection matrix
		cubemapShader.SetMatrixUniform4f("view", glm::value_ptr(view));
		cubemapShader.SetMatrixUniform4f("projection", glm::value_ptr(projection));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS); // set depth function back to default

		glBindVertexArray(0);
		
		#pragma endregion

		glBindVertexArray(fontVAO);

		RenderText(shader, fps_str, 0.0f, 580.0f, .5f, glm::vec3(1.0, 0.8f, 0.2f));

		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		glBindVertexArray(0);

		// Check and call events and swap the buffers
		glfwPollEvents(); // checks if any events are triggered (like keyboard input or mouse movement events)
		glfwSwapBuffers(window); // swap the color buffer
	}

	std::cout << "Max FPS : " << maxFPS << std::endl;
	std::cout << "Min FPS : " << minFPS << std::endl;
	std::cout << "Window is closed!" << std::endl;
	glfwTerminate(); // Clear all allocated resources
	return 0;
}
