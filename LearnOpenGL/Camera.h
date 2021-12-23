#pragma once

#include "glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext/matrix_clip_space.hpp"

enum class CameraType
{
	ORTHGRAPHIC,
	PERSPECTIVE
};

enum class CameraMoveDir
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up, int windowWidth, int windowHeight, bool isTrueFPS = false);

	void ProcessMouseMove(float xPos, float yPos);
	void ProcessMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
	void ProcessKeyboard(CameraMoveDir moveDir, float offset);
	glm::mat4 LookAt();
	glm::mat4 SetProjection(CameraType type, int windowWidth, int windowHeight, float nearPlaneDist, float farPlaneDist);

	glm::vec3 GetPosition();
	glm::vec3 GetFront();

private:
	// Variables
	glm::vec3 _cameraPos;
	glm::vec3 _cameraFront;
	glm::vec3 _cameraRight;
	glm::vec3 _cameraUp;

	float _yaw; 
	float _pitch;
	float _lastX; 
	float _lastY;

	float _fov;

	bool _firstMouse;
	bool _isTrueFPS;

	// Member Functions
	void inline MoveForward(float offset);
	void inline MoveBackward(float offset);
	void inline MoveLeft(float offset);
	void inline MoveRight(float offset);
	void PrintMatrix4x4(std::string nameOfMatrix, glm::mat4 matrix);

};

