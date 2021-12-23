#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 center, glm::vec3 up, int windowWidth, int windowHeight, bool trueFPS)
{
	// Camera position
	_cameraPos = pos;
	// Set Camera front to target to point camera at it
	_cameraFront = glm::normalize(center - pos);
	// Camera right is x-axis of the camera
	_cameraRight = glm::normalize(glm::cross(_cameraFront, up));
	// Camera up is y-axis of the camera
	_cameraUp = glm::cross(_cameraRight, _cameraFront);


	
	_yaw = -90.0f;
	_pitch = 0.0f;
	_lastX = (float)windowWidth / 2;
	_lastY = (float)windowHeight / 2;

	_firstMouse = true;
	_isTrueFPS = trueFPS;

	_fov = 45.0f;

	std::cout << "=========================================\n";
	std::cout << "World Center	: (" << center.x << ", " << center.y << ", " << center.z << ")\n";
	std::cout << "Cam Pos		: (" << _cameraPos.x << ", " << _cameraPos.y << ", " << _cameraPos.z << ")\n";
	std::cout << "Cam Front	: (" << _cameraFront.x << ", " << _cameraFront.y << ", " << _cameraFront.z << ")\n";
	std::cout << "Cam Right	: (" << _cameraRight.x << ", " << _cameraRight.y << ", " << _cameraRight.z << ")\n";
	std::cout << "Cam Up		: (" << _cameraUp.x << ", " << _cameraUp.y << ", " << _cameraUp.z << ")\n";

}

void Camera::ProcessMouseMove(float xPos, float yPos)
{

	if (_firstMouse) // initially set to true
	{
		_lastX = xPos;
		_lastY = yPos;
		_firstMouse = false;
	}

	float xoffset = xPos - _lastX;
	float yoffset = _lastY - yPos;

	_lastX = xPos;
	_lastY = yPos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	_yaw += xoffset;
	_pitch += yoffset;

	if (_pitch > 89.0f)
		_pitch = 89.0f;
	if (_pitch < -89.0f)
		_pitch = -89.0f;


	// std::cout << "Yaw : (" << _yaw << ")\n";
	// std::cout << "Pitch : (" << _pitch << ")\n";
	

	glm::vec3 direction;
	direction.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	direction.y = sin(glm::radians(_pitch));
	direction.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));

	_cameraFront = glm::normalize(direction);
}

void Camera::ProcessMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	_fov -= (float)yoffset;
	if (_fov < 1.0f)
		_fov = 1.0f;
	if (_fov > 45.0f)
		_fov = 45.0f;
}

void Camera::ProcessKeyboard(CameraMoveDir moveDir, float offset)
{
	switch (moveDir)
	{
		case CameraMoveDir::FORWARD:
			MoveForward(offset);
			break;
		case CameraMoveDir::BACKWARD:
			MoveBackward(offset);
			break;
		case CameraMoveDir::LEFT:
			MoveLeft(offset);
			break;
		case CameraMoveDir::RIGHT:
			MoveRight(offset);
			break;
		default:
			break;
	}

	if (_isTrueFPS)
		_cameraPos.y = 0;

	// std::cout << "Cam Pos : (" << _cameraPos.x << ", " << _cameraPos.y << ", " << _cameraPos.z << ")\n";
}

void Camera::MoveForward(float offset)
{
	_cameraPos += offset * _cameraFront;
}

void Camera::MoveBackward(float offset)
{
	_cameraPos -= offset * _cameraFront;
}

void Camera::MoveLeft(float offset)
{
	_cameraPos -= glm::normalize(glm::cross(_cameraFront, _cameraUp)) * offset;
}

void Camera::MoveRight(float offset)
{
	_cameraPos += glm::normalize(glm::cross(_cameraFront, _cameraUp)) * offset;
}

void Camera::PrintMatrix4x4(std::string name, glm::mat4 matrix)
{
	std::cout << name << "\n";
	for (int col = 0; col < matrix[0].length(); col++)
	{
		for (int row = 0; row < matrix[1].length(); row++)
		{
			std::cout << "[" << matrix[col][row] << "]";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

glm::mat4 Camera::LookAt()
{
	auto lookat = glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);

	// PrintMatrix4x4("GLM::LookAt : ", lookat);

	return lookat;
}

glm::mat4 Camera::SetProjection(CameraType type, int windowWidth, int windowHeight, float nearPlaneDist, float farPlaneDist)
{
	switch (type)
	{
		case CameraType::ORTHGRAPHIC:
			return glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight, nearPlaneDist, farPlaneDist);
			break;
		case CameraType::PERSPECTIVE:
			return glm::perspective(glm::radians(_fov), (float)windowWidth / (float)windowHeight, nearPlaneDist, farPlaneDist);
			break;
		default:
			std::cout << "WARNING::CAMERA::SET_PROJECTION : Camera type not found!\n";
			break;
	}

	return glm::mat4();
}

glm::vec3 Camera::GetPosition()
{
	return _cameraPos;
}

glm::vec3 Camera::GetFront()
{
	return _cameraFront;
}
