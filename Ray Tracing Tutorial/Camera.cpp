#include "Camera.h"


Camera::Camera(){

	FOV = 45;
	aspectRatio = (GLfloat)(1);
	near = 0.01;
	far = 1000;

	resetMatrix();

}

Camera::Camera(float FOV, float aspectRatio, float near, float far) {

	Camera::FOV = (float)glm::radians(FOV);
	Camera::aspectRatio = (GLfloat)aspectRatio;
	Camera::near = near;
	Camera::far = far;

	resetMatrix();

}

void Camera::translate(float x, float y, float z) {

	matrix = glm::translate(matrix, glm::vec3(x, y, z));

	changed = true;

}

void Camera::rotate(float x, float y, float z) {

	// apply rotations one at a time
	matrix = glm::rotate(matrix, (float)glm::radians(x), { 1, 0, 0 });
	matrix = glm::rotate(matrix, (float)glm::radians(y), { 0, 1, 0 });
	matrix = glm::rotate(matrix, (float)glm::radians(z), { 0, 0, 1 });

	changed = true;

}

void Camera::resetMatrix() {

	// create a new matrix and replace the old one with it
	glm::mat4 new_matrix(1);
	matrix = new_matrix;

	// apply perspective info
	matrix = glm::perspective(FOV, aspectRatio, near, far) * matrix;

	changed = true;

}

void Camera::calculateInverseMatrix() {

	inverseMatrix = glm::inverse(matrix);

}

//* GETTERS
glm::mat4 Camera::getProjectionMatrix() {
	return matrix;
}

glm::mat4 Camera::getInverseMatrix() {
	if (changed) { calculateInverseMatrix(); }
	return inverseMatrix;
}

float* Camera::getInverseMatrixPTR() {
	if (changed) { calculateInverseMatrix(); }
	return glm::value_ptr(inverseMatrix);
}