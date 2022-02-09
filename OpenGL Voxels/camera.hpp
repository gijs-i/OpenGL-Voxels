#include "glm/glm.hpp"

#ifndef CAMERA_H
#define CAMERA_H

class Camera {
public:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	glm::vec3 cameraPos = glm::vec3(0.0f, 25.0f, 40.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraDirection;

	float FOV = 45.0f;
	float cameraSpeed = 10.0f;
	float yaw = 0.0f;
	float pitch = 0.0f;

	unsigned int projectionUniform;
	unsigned int viewUniform;

	Camera(unsigned int shaderProgram) {
		projectionUniform = glGetUniformLocation(shaderProgram, "uProjection");
		projectionMatrix = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		viewUniform = glGetUniformLocation(shaderProgram, "uView");
		viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	}

	void update() {
		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projectionMatrix = glm::perspective(glm::radians(FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	}

	void updatePitchAndYaw(float mouseXOffset, float mouseYOffset) {

	}
};

#endif