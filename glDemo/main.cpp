
#include "core.h"

using namespace std;


// global variables

// Example exture object
GLuint playerTexture;
GLuint brickTexture;


// Window size
const unsigned int initWidth = 512;
const unsigned int initHeight = 512;

// Function prototypes
void renderScene();
void resizeWindow(GLFWwindow* window, int width, int height);
void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
void updateScene();
void drawL();
void drawScales();
void drawPolygon(int sides);


int main() {

	//
	// 1. Initialisation
	//
	

	// Initialise glfw and setup window
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_TRUE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);

	GLFWwindow* window = glfwCreateWindow(initWidth, initHeight, "Real-Time Computer Graphics", NULL, NULL);

	// Check window was created successfully
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	

	// Set callback functions to handle different events
	glfwSetFramebufferSizeCallback(window, resizeWindow); // resize window callback
	glfwSetKeyCallback(window, keyboardHandler); // Keyboard input callback


	// Initialise glew
	glewInit();

	
	// Setup window's initial size
	resizeWindow(window, initWidth, initHeight);

	// Initialise scene - geometry and shaders etc
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // setup background colour to be black


	//
	// Setup textures
	//

	
	// Load image file from disk
	auto textureImageFile = string("Assets\\Textures\\player1_ship.png");
	FIBITMAP* bitmap = FreeImage_Load(FIF_PNG, textureImageFile.c_str(), BMP_DEFAULT);

	if (bitmap) {

		// If image loaded, setup new texture object in OpenGL
		glGenTextures(1, &playerTexture); // can create more than 1!
		
		if (playerTexture) {

			glBindTexture(GL_TEXTURE_2D, playerTexture);

			// Setup texture image properties
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				FreeImage_GetWidth(bitmap),
				FreeImage_GetHeight(bitmap),
				0,
				GL_BGRA,
				GL_UNSIGNED_BYTE,
				FreeImage_GetBits(bitmap));

			// Setup texture filter and wrap properties
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}

		// Once the texture has been setup, the image data is copied into OpenGL.  We no longer need the originally loaded image
		FreeImage_Unload(bitmap);
	}
	else {

		cout << "Error loading image!" << endl;
	}

	
	//Load image file from disk
	textureImageFile = string("Assets\\Textures\\brick2.png");
	bitmap = FreeImage_Load(FIF_PNG, textureImageFile.c_str(), BMP_DEFAULT);

	if (bitmap) {

		// If image loaded, setup new texture object in OpenGL
		glGenTextures(1, &brickTexture); // can create more than 1!

		if (brickTexture) {

			glBindTexture(GL_TEXTURE_2D, brickTexture);

			// Setup texture image properties
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				FreeImage_GetWidth(bitmap),
				FreeImage_GetHeight(bitmap),
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				FreeImage_GetBits(bitmap));

			// Setup texture filter and wrap properties
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}

		// Once the texture has been setup, the image data is copied into OpenGL.  We no longer need the originally loaded image
		FreeImage_Unload(bitmap);
	}
	else {

		cout << "Error loading image!" << endl;
	}
	
	//
	// 2. Main loop
	// 
	

	// Loop while program is not terminated.
	while (!glfwWindowShouldClose(window)) {

		updateScene();
		renderScene();						// Render into the current buffer
		glfwSwapBuffers(window);			// Displays what was just rendered (using double buffering).

		// Poll events (key presses, mouse events)
		// glfwWaitEvents();				// Use this if no animation.
		// glfwWaitEventsTimeout(1.0/60.0);	// Use this to animate at 60 frames/sec (timing is NOT reliable)
		glfwPollEvents();					// Use this version when animating as fast as possible
	}

	glfwTerminate();
	return 0;
}



// renderScene - function to render the current scene
void renderScene()
{
	
	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*
	// Render objects here...
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, playerTexture);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-0.5f, 0.5f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(0.5f, 0.5f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(0.5f, -0.5f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-0.5f, -0.5f);

	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	*/
	drawScales();
}

void drawL()
{
	glBegin(GL_TRIANGLE_FAN);

	glVertex2f(-0.8f, -0.6f);
	glVertex2f(-0.8f, 0.6f);
	glVertex2f(-0.5f, 0.6f);

	glVertex2f(-0.5f, -0.3f);
	glVertex2f(0.6f, -0.3f);
	glVertex2f(0.6f, -0.6f);

	glEnd();
}

void drawScales()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, brickTexture);

	glBegin(GL_TRIANGLES);

	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-0.3f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(0.3f, -1.0f);
	glTexCoord2f(0.5f, 1.0f);
	glVertex2f(0.0f, 0.5f);

	glVertex2f(-0.5f, 0.6f);
	glVertex2f(0.5f, 0.6f);
	glVertex2f(0.0f, 0.5f);

	glVertex2f(-0.5f, 0.6f);
	glVertex2f(0.5f, 0.6f);
	glVertex2f(0.0f, 0.7f);

	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINE_LOOP);

	glVertex2f(-0.5f, 0.6f);
	glVertex2f(-0.7f, 0.2f);
	glVertex2f(-0.3f, 0.2f);

	glEnd();

	glBegin(GL_LINE_LOOP);

	glVertex2f(0.5f, 0.6f);
	glVertex2f(0.7f, 0.2f);
	glVertex2f(0.3f, 0.2f);

	glEnd();

	glBegin(GL_QUADS);

	glVertex2f(0.7f, -0.1f);
	glVertex2f(0.7f, 0.2f);
	glVertex2f(0.3f, 0.2f);
	glVertex2f(0.3f, -0.1f);

	glVertex2f(-0.7f, -0.1f);
	glVertex2f(-0.7f, 0.2f);
	glVertex2f(-0.3f, 0.2f);
	glVertex2f(-0.3f, -0.1f);

	glEnd();

}

void drawPolygon(int sides)
{
	glShadeModel(GL_SMOOTH);

	const float pi = 3.1415926f;
	const float thetaStep = 2.0f * pi / float(sides);

	glBegin(GL_TRIANGLE_FAN);

	glVertex2f(0.0f, 0.0f);

	for (int i = 0; i <= sides; i++)
	{

		float x = cosf(thetaStep * float(i));
		float y = sinf(thetaStep * float(i));

		glVertex2f(x, y);

	};

	glEnd();

};


// Function to call when window resized
void resizeWindow(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);		// Draw into entire window
}


// Function to call to handle keyboard input
void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {

		// check which key was pressed...
		switch (key)
		{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, true);
				break;

			default:
			{
			}
		}
	}
	else if (action == GLFW_RELEASE) {
		// handle key release events
	}
}


// Function called to animate elements in the scene
void updateScene() {
}

