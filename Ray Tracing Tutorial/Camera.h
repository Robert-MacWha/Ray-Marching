#pragma once

#define GLEW_STATIC
#include <GL/glew.h>    // include GLEW and newest version of GL on windows
#include <GLFW/glfw3.h> // GL helper library
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>  // required for glm::value_ptr (used when sending matrix to compute shader's uniform)
#include <glm/gtx/transform.hpp> // required for glm::perpsective (used when creating the view matrix)

class Camera
{
private:

	float FOV;
	GLfloat aspectRatio;
	float near;
	float far;

	glm::mat4 matrix;
	glm::mat4 inverseMatrix;

	bool changed;

public:
	Camera();
	Camera(float FOV, float aspectRatio, float near, float far);

	void translate(float x, float y, float z);
	void rotate(float x, float y, float z);

	void resetMatrix();
	void calculateInverseMatrix();

	// getters
	glm::mat4 getProjectionMatrix(); // projection matrix is used to convert world space to camera space 
	glm::mat4 getInverseMatrix();    // inverse projection matrix is used to converte camera space to world space
	float* getInverseMatrixPTR();    // pointer to inverse projection matrix is used when pasing to compute shaders

	float getNearPlane() { return near; }
	float getFarPlane()  { return far; }

};

