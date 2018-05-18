#include "Window.h"


#include <string>


#include "../visualization/Axis.h"
#include "../visualization/Grid.h"


seca::viewer::Window::~Window()
{
	ImGui_ImplGlfwGL3_Shutdown();
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

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

	camera = new render::Camera();
	camera->SetPanScale(0.01f);
	camera->SetDollyStartPosition(-1.5f);
	camera->SetDollyScale(0.1f);
	camera->SetTrackballScale(0.1f);
	camera->SetCenterOfRotation(glm::vec3(0, 0, 0));
	camera->Resize(m_windowSizeW, m_windowSizeH);

	glfwSetWindowUserPointer(m_window, this);
	glfwSetMouseButtonCallback(m_window, OnMouseButtonCallback);
	glfwSetCursorPosCallback(m_window, CursorPositionCallback);
	glfwSetScrollCallback(m_window, OnScrollCallback);
	glfwSetWindowSizeCallback(m_window, WindowSizeCallback);
	glfwSetDropCallback(m_window, SetDropCallback);

	render = new render::RenderObject();
	render->setup(m_window);
	
	// create default object
	visualization::Axis axis;
	axis.createAxisObject();
	render->loadObject(axis.getAxisObject());

	visualization::Grid grid;
	grid.createGridObject();
	render->loadObject(grid.getGridObject());
}

void seca::viewer::Window::Run()
{
	// set UI
	ui::StatusUI *uiStatus = new ui::StatusUI();
	uiStatus->param_clearColor = &param_clearColor;
	m_uis.push_back(uiStatus);

	// render
	while (!glfwWindowShouldClose(m_window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, m_windowSizeW, m_windowSizeH);
		glClearColor(
			param_clearColor.x,
			param_clearColor.y,
			param_clearColor.z,
			param_clearColor.w
		);

		// Camera
		camera->ContinueRotation();

		// render
		render->render(camera->GetWorldViewMatrix());

		// UI
		ImGui_ImplGlfwGL3_NewFrame();
		for (ui::CommonUI *ui : m_uis) { ui->render(); }
		ImGui::Render();

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}

void seca::viewer::Window::OnMouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	Window *srcWindow = (Window*)glfwGetWindowUserPointer(window);

	ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);

	glfwGetWindowUserPointer(window);

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		srcWindow->camera->StartMouseRotation(xpos, ypos);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		srcWindow->camera->EndMouseRotation(xpos, ypos);
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		srcWindow->camera->StartMousePan(xpos, ypos);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		srcWindow->camera->EndMousePan(xpos, ypos);
	}
}

void seca::viewer::Window::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	Window *srcWindow = (Window*)glfwGetWindowUserPointer(window);

	srcWindow->camera->ProcessMouseMotion(xpos, ypos);
}

void seca::viewer::Window::OnScrollCallback(GLFWwindow * window, double offsetx, double offsety)
{
	Window *srcWindow = (Window*)glfwGetWindowUserPointer(window);

	ImGui_ImplGlfwGL3_ScrollCallback(window, offsetx, offsety);

	glfwGetWindowUserPointer(window);

	srcWindow->camera->UpdateDolly(offsety);
}

void seca::viewer::Window::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
	Window *srcWindow = (Window*)glfwGetWindowUserPointer(window);

	SECA_CONSOLE_INFO("resize window : {} x {}", width, height);

	srcWindow->camera->Resize(width, height);
}

void seca::viewer::Window::SetDropCallback(GLFWwindow * window, int count, const char** paths)
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