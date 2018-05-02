#include "WindowCamera.h"


seca::viewer::WindowCamera::WindowCamera()
{
	m_camera = new render::Camera();
	m_camera->Initialize(glm::vec3(0.0f, 3.0f, 0.0f), 1.0f);
}

seca::viewer::WindowCamera::~WindowCamera()
{
	delete(m_camera);
}

void seca::viewer::WindowCamera::updateMouse(GLFWwindow *window)
{
	int w, h;
	glfwGetWindowSize(window, &w, &h);

	double curX, curY;
	glfwGetCursorPos(window, &curX, &curY);

	m_dx = (curX - m_prevX) / (double)w;
	m_dy = (curY - m_prevY) / (double)h;
	m_prevX = curX;
	m_prevY = curY;
	m_scrollX = 0;
	m_scrollY = 0;
}

void seca::viewer::WindowCamera::setWindow(GLFWwindow *_window)
{
	m_window = _window;

	int w, h;
	glfwGetWindowSize(_window, &w, &h);
	m_camera->SetSize((float)w, (float)h);
}

glm::mat4x4 seca::viewer::WindowCamera::getMVP()
{
	auto world = glm::mat4(1.0);
	auto view = m_camera->GetViewMatrix();
	auto proj = m_camera->GetProjectionMatrix();
	return (proj * view * world);
}

void seca::viewer::WindowCamera::update()
{
	if (m_mouseLockMode == MouseLockMode::RequestLock)
	{
		if (ImGui::GetIO().WantCaptureMouse)
		{
			m_mouseLockMode = MouseLockMode::None;
			m_cameraMode = CameraMode::None;
		}
		else
		{
			m_mouseLockMode = MouseLockMode::Lock;
		}
	}

	if (m_mouseLockMode == MouseLockMode::Lock && m_cameraMode != CameraMode::None)
	{
		if (m_cameraMode == CameraMode::Orbit)
		{
			m_camera->Orbit((float)m_dx, (float)m_dy);
		}
		if (m_cameraMode == CameraMode::Dolly)
		{
			m_camera->Dolly((float)m_dx + (float)m_dy);
		}
		if (m_cameraMode == CameraMode::Pan)
		{
			m_camera->Pan((float)m_dx, (float)m_dy);
		}
	}

	if (m_scrollY != 0 && (!ImGui::GetIO().WantCaptureMouse))
	{
		m_camera->Dolly((float)m_scrollY * 0.1f);
	}

	updateMouse(m_window);

	m_camera->UpdateMatrix();
}

void seca::viewer::WindowCamera::OnScroll(double offsetx, double offsety)
{
	m_scrollX = offsetx;
	m_scrollY = offsety;
}

void seca::viewer::WindowCamera::OnScrollStub(GLFWwindow * window, double offsetx, double offsety)
{
	ImGui_ImplGlfwGL3_ScrollCallback(window, offsetx, offsety);

	glfwGetWindowUserPointer(window);

	OnScroll(offsetx, offsety);
}

void seca::viewer::WindowCamera::OnMouseButton(GLFWwindow * m_window, int button, int action, int mods)
{
	auto prevCameraMode = m_cameraMode;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		m_cameraMode = CameraMode::Orbit;
		if (glfwGetKey(m_window, GLFW_KEY_Z) == GLFW_PRESS)
		{
			m_cameraMode = CameraMode::Orbit;
		}
		else if (glfwGetKey(m_window, GLFW_KEY_X) == GLFW_PRESS)
		{
			m_cameraMode = CameraMode::Pan;
		}
		else if (glfwGetKey(m_window, GLFW_KEY_C) == GLFW_PRESS)
		{
			m_cameraMode = CameraMode::Dolly;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		m_cameraMode = CameraMode::Dolly;
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
	{
		m_cameraMode = CameraMode::Pan;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT ||
		button == GLFW_MOUSE_BUTTON_RIGHT ||
		button == GLFW_MOUSE_BUTTON_MIDDLE
		)
	{
		if (action == GLFW_RELEASE)
		{
			m_cameraMode = CameraMode::None;
			m_mouseLockMode = MouseLockMode::None;
		}
	}
	if (prevCameraMode == CameraMode::None && m_cameraMode != CameraMode::None)
	{
		m_mouseLockMode = MouseLockMode::RequestLock;
	}
}

void seca::viewer::WindowCamera::OnMouseButtonStub(GLFWwindow * window, int button, int action, int mods)
{
	ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);

	glfwGetWindowUserPointer(window);

	OnMouseButton(window, button, action, mods);
}