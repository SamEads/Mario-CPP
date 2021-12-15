#include "Camera.hpp"

Camera::Camera(float x, float y)
{
	position.x = x;
	position.y = y;
}

void Camera::setPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}