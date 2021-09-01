// GLEW
#define GLEW_STATIC
#include <GL/glew.h>    // include GLEW and newest version of GL on windows
#include <GLFW/glfw3.h> // GL helper library
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>  // required for glm::value_ptr (used when sending matrix to compute shader's uniform)
#include <glm/gtx/transform.hpp> // required for glm::perpsective (used when creating the view matrix)

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "DisplayManager.h"
#include "TextureManager.h"
#include "Loader.h"
#include "Renderer.h"
#include "Camera.h"
#include "RawModel.h"
#include "TextureModel.h"
#include "ComputeShader.h"
#include "StaticShader.h"
#include "TextureShader.h"

using namespace std;

const int WIDTH = 960;
const int HEIGHT = 720;

int frame = 0;

//! Code for compute shaders mostly comes from this article: http://blog.three-eyed-games.com/2018/05/03/gpu-ray-tracing-in-unity-part-1/

int main() {

	//* INITIALIZE GL
	DisplayManager::createDisplay(WIDTH, HEIGHT, "Ray marching");

	//* INITIALIZE CUSTOMS
	Loader loader = Loader();
	Renderer renderer = Renderer();
	TextureShader textureShader = TextureShader();
	ComputeShader computeShader = ComputeShader("rayTracingShader.glsl");

	//* CREATE MODEL
	// create the texture
	GLuint renderTextureID = TextureManager::createTexture(WIDTH, HEIGHT, 0);

	// create the quad
	RawModel rectModel = loader.defaultQuad();
	TextureModel texturedRectModel = TextureModel(rectModel, renderTextureID);

	//* CREATE THE CAMERA
	Camera camera = Camera(90, (float)WIDTH / HEIGHT, 0.01, 100);
	camera.rotate(-20, 0, 0);
	camera.translate(0, 5, -1);
	
	//* MAIN GAME LOOP
	while (!DisplayManager::requestWindowClose()) {

		// clear the display
		DisplayManager::clearDisplay();

		// input management
		if (DisplayManager::getKey(GLFW_KEY_R)) {
			textureShader.reload();
			computeShader.reload();
		}

		// set the compute shader as the active one
		computeShader.use();

		// send uniforms to the shader
		glUniform1f(computeShader.getUniformLocation("frame"), frame);

		glUniform1f(computeShader.getUniformLocation("near"), 0.01);
		glUniform1f(computeShader.getUniformLocation("far"), 100);
		glUniformMatrix4fv(computeShader.getUniformLocation("projection_inv"), 1, GL_FALSE, camera.getInverseMatrixPTR());
		glUniform1i(computeShader.getUniformLocation("bounce_limit"), 5);

		// dispatch the shader
		computeShader.dispatch(WIDTH / 16, HEIGHT / 16, 1);

		// apply the model shader and render it
		textureShader.start();
		renderer.Render(texturedRectModel);

		// push the buffer to the window
		DisplayManager::updateDisplay();

		// increment the frame counter
		frame += 1;

	}

	//* CLEAN UP
	// close GL context and other GLFW resources
	DisplayManager::closeDisplay();
	return 0;

}