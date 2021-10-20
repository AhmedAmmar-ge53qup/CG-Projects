#include "Camera.h"

const float FOV = 45.0f;

Camera::Camera()
	:mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mTargetPos(glm::vec3(0.0f, 0.0f, 0.0f)),
	mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	mRight(glm::vec3(0.0f, 0.0f, 0.0f)),
	World_Up(glm::vec3(0.0f, 1.0f, 0.0f)),
	mYaw(glm::pi<float>()),
	mPitch(0.0f),
	mFOV(FOV)
{}


glm::mat4 Camera::getViewMatrix() const{
	return glm::lookAt(mPosition, mTargetPos, mUp);
}
const glm::vec3& Camera::getLook() const {
	return mLook;
}
const glm::vec3& Camera::getRight() const{
	return mRight;
}
const glm::vec3& Camera::getUp() const {
	return mUp;
}


FPSCamera::FPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = glm::pi<float>(), float pitch = 0)
{
	mPosition = position;
	mYaw = yaw;
	mPitch = pitch;
}
FPSCamera::FPSCamera(glm::vec3 position, glm::vec3 target)
{
	mPosition = position;
	mTargetPos = target;
	glm::vec3 lookDir = position - target;

	mYaw = atan2(lookDir.x, lookDir.z) + glm::pi<float>();
	mPitch = -atan2(lookDir.y, sqrt(lookDir.x * lookDir.x + lookDir.z * lookDir.z));
}

void FPSCamera::setPosition(const glm::vec3& position) {
	mPosition = position;
}
// Not doing rolling!! (z-axis)
void FPSCamera::rotate(float yaw, float pitch) 
{
	mYaw += glm::radians(yaw);
	mPitch += glm::radians(pitch);

	// Add Constraints on values, to remain in range
	mPitch = glm::clamp(mPitch, -glm::pi<float>()/2.0f + 0.1f, glm::pi<float>()/2.0f - 0.1f);

	if (mYaw > glm::two_pi<float>())
		mYaw -= glm::two_pi<float>();
	else if (mYaw < 0.0f)
		mYaw += glm::two_pi<float>();

	updateCameraVectors();
}
void FPSCamera::move(const glm::vec3& offsetPos) {
	
}