#ifndef SECA_VIEWER_WINDOW_H_
#define SECA_VIEWER_WINDOW_H_


#include <list>
#include <memory>


#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>


#include "render/Camera.h"
#include "render/Object.h"
#include "render/RenderObject.h"


#include "WindowCamera.h"


#include "../ui/CommonUI.h"
#include "../ui/StatusUI.h"


namespace seca
{
	namespace viewer
	{
		class Window
		{
		public:
			char windowTitle[64] = "SECA";
			ImVec4 param_clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		private:
			const int m_windowSizeW = 1280;
			const int m_windowSizeH = 1024;

			GLFWwindow *m_window;

			WindowCamera *m_windowCamera;

			std::list<ui::CommonUI*> m_uis;

			render::RenderObject *render;


		public:
			Window();
			~Window();

			void run()
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
					m_windowCamera->update();
					
					// render
					render->render(m_windowCamera->getMVP());

					// UI
					ImGui_ImplGlfwGL3_NewFrame();
					for (ui::CommonUI *ui : m_uis) { ui->render(); }
					ImGui::Render();

					glfwSwapBuffers(m_window);
					glfwPollEvents();
				}
			}

			static void OnScrollStub(GLFWwindow * window, double offsetx, double offsety);
			static void OnMouseButtonStub(GLFWwindow * window, int button, int action, int mods);
			static void SetDropStub(GLFWwindow * window, int count, const char** paths);
		};
	}
}

#endif // SECA_VIEWER_WINDOW_H_
