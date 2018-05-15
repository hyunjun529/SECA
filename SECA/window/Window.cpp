#include "Window.h"


#include <string>


#include "../visualization/Axis.h"
#include "../visualization/Grid.h"


seca::viewer::Window::Window()
{
	if (!glfwInit()) assert("failed glfwinit");

	m_window = glfwCreateWindow(m_windowSizeW, m_windowSizeH, windowTitle, NULL, NULL);

	if (!m_window)
	{
		glfwTerminate();
		assert("failed create glfw");
	}

	glfwMakeContextCurrent(m_window);

	if (gl3wInit())
	{
		assert("failed gl3winit");
	}

	ImGui_ImplGlfwGL3_Init(m_window, true);
	ImGui::StyleColorsClassic();

	m_windowCamera = new WindowCamera();
	m_windowCamera->setWindow(m_window);

	glfwSetWindowUserPointer(m_window, this);
	glfwSetMouseButtonCallback(m_window, OnMouseButtonStub);
	glfwSetScrollCallback(m_window, OnScrollStub);
	glfwSetDropCallback(m_window, SetDropStub);

	render = new render::RenderObject();
	render->setup(m_window);
	
	// need to move
	visualization::Axis axis;
	axis.createAxisObject();
	render->loadObject(axis.getAxisObject());

	visualization::Grid grid;
	grid.createGridObject();
	render->loadObject(grid.getGridObject());
}

seca::viewer::Window::~Window()
{
	ImGui_ImplGlfwGL3_Shutdown();
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void seca::viewer::Window::OnScrollStub(GLFWwindow * window, double offsetx, double offsety)
{
	Window *srcWindow = (Window*)glfwGetWindowUserPointer(window);

	ImGui_ImplGlfwGL3_ScrollCallback(window, offsetx, offsety);

	glfwGetWindowUserPointer(window);

	srcWindow->m_windowCamera->OnScroll(offsetx, offsety);
}

void seca::viewer::Window::OnMouseButtonStub(GLFWwindow * window, int button, int action, int mods)
{
	Window *srcWindow = (Window*)glfwGetWindowUserPointer(window);

	ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);

	glfwGetWindowUserPointer(window);

	srcWindow->m_windowCamera->OnMouseButton(window, button, action, mods);
}

void seca::viewer::Window::SetDropStub(GLFWwindow * window, int count, const char** paths)
{
	Window *srcWindow = (Window*)glfwGetWindowUserPointer(window);

	std::string str = paths[0];
	std::string::size_type pos = 0u;
	std::string oldStr = "\\";
	std::string newStr = "\\\\";
	while ((pos = str.find(oldStr, pos)) != std::string::npos) {
		str.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}

	std::size_t found = str.find_last_of("/\\\\");
	std::string path = str.substr(0, found + 1);
	std::string file = str.substr(found + 1);

	SECA_CONSOLE_INFO("{}, {}", file.c_str(), path.c_str());

	srcWindow->render->loadOBJObject(file.c_str(), path.c_str());
}

//render->loadOBJObject("kizunaai.obj", "..//resource//kizunaai//");
//if (m_inputImgui->funcLoad)
//{
//	switch (m_inputImgui->param_load_obj)
//	{
//	case 1:
//		load("cube.obj", "..//resource//cube//");
//		break;
//	case 2:
//		load("bunny.obj", "..//resource//bunny//");
//		break;
//	case 3:
//		load("teapot.obj", "..//resource//teapot//");
//		break;
//	case 4:
//		load("capsule.obj", "..//resource//capsule//");
//		break;
//	case 5:
//		load("white_oak.obj", "..//resource//white_oak//");
//		break;
//	case 6:
//		load("kizunaai.obj", "..//resource//kizunaai//");
//		break;
//	}
//	m_inputImgui->funcLoad = false;
//}