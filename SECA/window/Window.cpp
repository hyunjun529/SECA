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

	// loader
	loader = new format::Loader();
}

void seca::viewer::Window::Run()
{
	// set UI
	ui::StatusUI *uiStatus = new ui::StatusUI();
	uiStatus->param_clearColor = &param_clearColor;
	m_uis.push_back(uiStatus);

	ui::ObjectListUI *ui_object_list = new ui::ObjectListUI();
	ui_object_list->m_objectLists = &m_objects;
	m_uis.push_back(ui_object_list);

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
		double xpos, ypos;
		glfwGetCursorPos(m_window, &xpos, &ypos);
		camera->ProcessMouseMotion(xpos, ypos);
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

void seca::viewer::Window::setWindowSize(int width, int height)
{
	m_windowSizeW = width;
	m_windowSizeH = height;
}

void seca::viewer::Window::OnMouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	Window *srcWindow = (Window*)glfwGetWindowUserPointer(window);

	ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	if(srcWindow != nullptr)
	{
		if (action == GLFW_PRESS && !ImGui::IsMouseHoveringAnyWindow())
		{
			if (button == GLFW_MOUSE_BUTTON_RIGHT)
			{
				srcWindow->camera->StartMouseRotation(xpos, ypos);
			}
			if (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_MIDDLE)
			{
				srcWindow->camera->StartMousePan(xpos, ypos);
			}
		}
		else if (action == GLFW_RELEASE)
		{
			if (button == GLFW_MOUSE_BUTTON_RIGHT)
			{
				srcWindow->camera->EndMouseRotation(xpos, ypos);
			}
			if (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_MIDDLE)
			{
				srcWindow->camera->EndMousePan(xpos, ypos);
			}
		}
	}
}

void seca::viewer::Window::OnScrollCallback(GLFWwindow * window, double offsetx, double offsety)
{
	Window *srcWindow = (Window*)glfwGetWindowUserPointer(window);

	ImGui_ImplGlfwGL3_ScrollCallback(window, offsetx, offsety);

	if (srcWindow != nullptr)
	{
		srcWindow->camera->UpdateDolly(offsety);
	}
}

void seca::viewer::Window::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
	Window *srcWindow = (Window*)glfwGetWindowUserPointer(window);

	SECA_CONSOLE_INFO("resize window : {} x {}", width, height);

	srcWindow->camera->Resize(width, height);
	srcWindow->setWindowSize(width, height);
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

	GLsizei found = str.find_last_of("/\\\\");
	std::string path = str.substr(0, found + 1);
	std::string file = str.substr(found + 1);

	GLsizei found_dot = str.find_last_of(".");
	std::string extension = str.substr(found_dot + 1);

	SECA_CONSOLE_INFO("{}, {}", file.c_str(), path.c_str());

	if (!extension.empty() && extension.length() == 3)
	{
		render::Object target_object;

		// hmm...
		if (extension.compare("obj") == 0)
		{
			target_object = srcWindow->loader->loadOBJObject(file.c_str(), path.c_str());
			srcWindow->m_objects.push_back(target_object);
			srcWindow->render->loadObject(srcWindow->m_objects.back());
		}
		else if (extension.compare("pmx") == 0)
		{
			target_object = srcWindow->loader->loadPMXObject(file.c_str(), path.c_str());
			srcWindow->m_objects.push_back(target_object);
			srcWindow->render->loadObject(srcWindow->m_objects.back());
		}
		else if (extension.compare("fbx") == 0)
		{
			target_object = srcWindow->loader->loadFBXObject(str.c_str());
			srcWindow->m_objects.push_back(target_object);
			srcWindow->render->loadObject(srcWindow->m_objects.back());
		}
		else
		{
			SECA_CONSOLE_ERROR("{}, {} extension is not support", file.c_str(), extension.c_str());
		}
	}
	else
	{
		SECA_CONSOLE_ERROR("{} need extension(.obj, .pmx, .fbx)", file.c_str());
	}
}