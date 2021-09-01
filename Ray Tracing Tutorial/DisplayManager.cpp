
#include "DisplayManager.h"

//* DEFINE STATICS
int DisplayManager::WIDTH = 640;
int DisplayManager::HEIGHT = 480;
const char* DisplayManager::WINDOW_TITLE = "New Window";

GLFWwindow* DisplayManager::WINDOW = nullptr;

//* DEFINE FUNCTIONS
void DisplayManager::createDisplay(int w, int h, const char* title) {

	// assign parameters
	WIDTH = w;
	HEIGHT = h;
	WINDOW_TITLE = title;

	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "Error: could not start GLFW3\n");
	}

	// specify window settings
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// create the window
	WINDOW = glfwCreateWindow(WIDTH, HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!WINDOW) {
		fprintf(stderr, "Error: could not open window with GLFW3\n");
	}
	glfwMakeContextCurrent(WINDOW);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	// tell GL to only draw onto a pixel if the shape is close to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS);    // specifies that smaller distances are 'closer'

}

void DisplayManager::clearDisplay() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void DisplayManager::updateDisplay() {

	// update other events like input handling 
	glfwPollEvents();

	// put the stuff we've been drawing onto the display
	glfwSwapBuffers(WINDOW);

}

bool DisplayManager::getKey(int key) {

	return glfwGetKey(WINDOW, key);

}

bool DisplayManager::requestWindowClose() {

	return glfwWindowShouldClose(WINDOW);

}

void DisplayManager::closeDisplay() {

	glfwTerminate();

}