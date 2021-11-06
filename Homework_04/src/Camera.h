#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {

public:
	glm::mat4 getViewMatrix() const;

	virtual void setPosition(const glm::vec3& position) {}
	virtual void rotate(float yaw, float pitch) {}
	virtual void moveCamera(const glm::vec3& offsetPos) {}

	const glm::vec3& getLook() const;
	const glm::vec3& getRight() const;
	const glm::vec3& getUp() const;

	float getFOV() const { return mFOV; }
	void setFOV(float fov) { mFOV = fov; }

protected:
	Camera();
	virtual void updateCameraVectors() {}


	glm::vec3 mPosition;
	glm::vec3 mTargetPos;
	glm::vec3 mLook;	// Forward Vector
	glm::vec3 mUp, mRight;
	const glm::vec3 World_Up; // world up

	//camera parameter
	float mYaw, mPitch, mFOV;
};

// Implementation of FPS Camera
class FPSCamera : public Camera 
{
public:
	FPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = glm::pi<float>(), float pitch = 0.0f);
	FPSCamera(glm::vec3 position, glm::vec3 target);

	virtual void setPosition(const glm::vec3& position);
	virtual void rotate(float yaw, float pitch);	// Not doing rolling!! (z-axis)
	virtual void move(const glm::vec3& offsetPos);

private:
	void updateCameraVectors();
};


#endif //CAMERA_H