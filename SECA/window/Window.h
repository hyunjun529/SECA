#ifndef SECA_VIEWER_WINDOW_H_
#define SECA_VIEWER_WINDOW_H_


#include <memory>
#include <list>
#include <vector>


#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>


#include "render/Camera.h"
#include "render/Object.h"
#include "render/RenderObject.h"

#include "format/Loader.h"

#include "../ui/CommonUI.h"
#include "../ui/StatusUI.h"
#include "../ui/ObjectListUI.h"


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
			int m_windowSizeW = 1280;
			int m_windowSizeH = 1024;

			GLFWwindow *m_window;

			std::list<ui::CommonUI*> m_uis;

			render::RenderObject *render;
			render::Camera *camera;

			format::Loader *loader;

			std::vector<render::Object> m_objects;


		public:
			Window();
			~Window();

			void Run();

			void setWindowSize(int w, int h);

			static void OnMouseButtonCallback(GLFWwindow * window, int button, int action, int mods);
			static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
			static void OnScrollCallback(GLFWwindow * window, double offsetx, double offsety);
			static void WindowSizeCallback(GLFWwindow* window, int width, int height);
			static void SetDropCallback(GLFWwindow * window, int count, const char** paths);
		};
	}
}

#endif // SECA_VIEWER_WINDOW_H_
