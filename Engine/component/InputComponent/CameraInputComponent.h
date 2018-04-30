#ifndef KATA_COMPONENT_GLCAMERAINPUTCOMPONENT_H_
#define KATA_COMPONENT_GLCAMERAINPUTCOMPONENT_H_

#include "../InputComponent.h"

#include "../../render/World.h"
#include "../../render/Camera.h"

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>


namespace kata
{
	namespace component
	{
		class CameraInputComponent : public InputComponent
		{
		private:
			GLFWwindow *m_window;
			render::Camera *m_camera;

			double m_dx, m_dy;
			double m_prevX, m_prevY;
			double m_scrollX, m_scrollY;

			enum class CameraMode
			{
				None,
				Orbit,
				Dolly,
				Pan,
			};
			CameraMode m_cameraMode;

			enum class MouseLockMode
			{
				None,
				RequestLock,
				Lock,
			};
			MouseLockMode m_mouseLockMode;

			void updateMouse(GLFWwindow *window)
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


		public:
			CameraInputComponent()
			{
				m_camera = new render::Camera();
				m_camera->Initialize(glm::vec3(0.0f, 3.0f, 0.0f), 1.0f);
			}
			
			~CameraInputComponent()
			{
				delete(m_camera);
			}

			void setWindow(GLFWwindow *_window)
			{
				m_window = _window;
				
				int w, h;
				glfwGetWindowSize(_window, &w, &h);
				m_camera->SetSize((float)w, (float)h);
			}

			glm::mat4x4 getMVP()
			{
				auto world = glm::mat4(1.0);
				auto view = m_camera->GetViewMatrix();
				auto proj = m_camera->GetProjectionMatrix();
				return (proj * view * world);
			}

			void renderImGui()
			{
				glm::mat4x4 MVP = getMVP();

				ImGui::Begin("MVP");
				for (int i = 0; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						ImGui::Text("%d %d : %lf", i, j, MVP[i][j]);
					}
				}
				ImGui::End();

				ImGui::Begin("Mouse");
				ImGui::Text("m_dx = %lf", m_dx);
				ImGui::Text("m_dy = %lf", m_dy);
				ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
				ImGui::Text("m_prevX = %lf", m_prevX);
				ImGui::Text("m_prevY = %lf", m_prevY);
				ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
				ImGui::Text("m_cameraMode = %d", m_cameraMode);
				ImGui::End();

				ImGui::Begin("Camera Position");
				glm::vec3 ep = m_camera->GetEyePostion();
				ImGui::Text("x = %lf", ep.x);
				ImGui::Text("y = %lf", ep.y);
				ImGui::Text("z = %lf", ep.z);
				ImGui::End();

				ImGui::Render();
			}

			void update()
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

			void OnScroll(double offsetx, double offsety)
			{
				m_scrollX = offsetx;
				m_scrollY = offsety;
			}
			
			void OnMouseButton(GLFWwindow * m_window, int button, int action, int mods)
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

			// 아래 두 함수는 직접 호출하는게 아니라 컴포넌트 밖에서 따로 콜백을 관리해야됨
			void OnScrollStub(GLFWwindow * window, double offsetx, double offsety)
			{
				ImGui_ImplGlfwGL3_ScrollCallback(window, offsetx, offsety);

				glfwGetWindowUserPointer(window);

				OnScroll(offsetx, offsety);
			}

			void OnMouseButtonStub(GLFWwindow * window, int button, int action, int mods)
			{
				ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);

				glfwGetWindowUserPointer(window);

				OnMouseButton(window, button, action, mods);
			}
		};
	}
}

#endif // KATA_COMPONENT_GLCAMERAINPUTCOMPONENT_H_
