#include <GL/gl3w.h>
#include <GLFW/glfw3.h>


#include "util/Log.h"


#include "window/Window.h"


int main(int argc, char** argv)
{
	SECA_CONSOLE_INFO("HELL WORLD!");

	seca::viewer::Window *window = new seca::viewer::Window();
	
	window->Run();

	delete window;

	return 0;
}