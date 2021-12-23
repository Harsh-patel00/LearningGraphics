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
#include "glm/ext/matrix_clip_space.hpp"
#include "assimp/mesh.h"

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

#define print std::cout << 
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CAM_ORTHO (CameraType)0
#define CAM_PERSP  (CameraType)1

#pragma region Texture related global var

float mixValue = 0.4f;

#pragma endregion

#pragma region Time/Frame related gloabl var

float deltaTime = 0.0f;
float lastFrame = 0.0f;

#pragma endregion

#pragma region Camera realted global var

// Define camera
Camera ourCamera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 
	WINDOW_WIDTH, WINDOW_HEIGHT);

#pragma endregion

#pragma region Light related global var

// glm::vec3 lightPos(2.0f, 0.0f, 0.0f); // Set light src on +ve x-axis

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

	Shader textureShader(simpleVert, simpleFrag); // For object using textures
	Shader framebufferShader(simpleVS2, simpleFS2); // For framebuffer textures


	#pragma endregion

	#pragma region Textures

	const char* tex1Source = "./Resources/container.jpg";
	const char* tex2Source = "./Resources/marble.jpg";

	Texture tex1(tex1Source, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR);
	Texture tex2(tex2Source, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR);

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
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		// Enable Depth test
		glEnable(GL_DEPTH_TEST);

		// Rendering commands
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Use this color when clearing color buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer, z-buffer and stencil buffer

		// To render the output in wireframe mode, uncomment the below line
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		// Set the projection to perspective (Need to set this every frame as we are zooming-in and -out)
		glm::mat4 projection;
		projection = ourCamera.SetProjection(CAM_PERSP, WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 100.0f);

		// Set the view matrix (Need to set this every frame as we are moving the camera around)
		glm::mat4 view;
		view = ourCamera.LookAt();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.f));


		#pragma region Cube and Plane
		

		// Plane
		glBindVertexArray(planeVAO);

		textureShader.Use();

		tex2.Activate();

		textureShader.SetInt("texture1", tex2.textureNumber);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.001f));

		textureShader.SetMatrixUniform4f("model", glm::value_ptr(model));
		textureShader.SetMatrixUniform4f("view", glm::value_ptr(view));
		textureShader.SetMatrixUniform4f("projection", glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, 36);


		// 1st Cube
		glBindVertexArray(cubeVAO);
		
		tex1.Activate();

		textureShader.SetInt("texture1", tex1.textureNumber);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.f));
		textureShader.SetMatrixUniform4f("model", glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Second Cube
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, 0.f, 1.5f));
		textureShader.SetMatrixUniform4f("model", glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		#pragma endregion

		glBindVertexArray(0);

		#pragma region DefaultFramebuffer

		
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // Use default framebuffer
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.f));

		// Plane
		glBindVertexArray(planeVAO);

		textureShader.Use();

		tex2.Activate();

		textureShader.SetInt("texture1", tex2.textureNumber);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.001f));

		textureShader.SetMatrixUniform4f("model", glm::value_ptr(model));
		textureShader.SetMatrixUniform4f("view", glm::value_ptr(view));
		textureShader.SetMatrixUniform4f("projection", glm::value_ptr(projection));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Cube
		glBindVertexArray(cubeVAO);

		textureShader.Use();

		tex1.Activate();

		textureShader.SetInt("texture1", tex1.textureNumber);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.f));
		textureShader.SetMatrixUniform4f("model", glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Second Cube
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, 0.f, 1.5f));
		textureShader.SetMatrixUniform4f("model", glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);


		framebufferShader.Use();

		texColorBuffer.Activate();

		framebufferShader.SetInt("screenTexture", texColorBuffer.textureNumber);

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		#pragma endregion

		glBindVertexArray(0);

		// Check and call events and swap the buffers
		glfwPollEvents(); // checks if any events are triggered (like keyboard input or mouse movement events)
		glfwSwapBuffers(window); // swap the color buffer
	}

	std::cout << "Window is closed!" << std::endl;
	glfwTerminate(); // Clear all allocated resources
	return 0;
}
