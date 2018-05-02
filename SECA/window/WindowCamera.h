#ifndef SECA_VIEWER_WINDOW_CAMERA_H_
#define SECA_VIEWER_WINDOW_CAMERA_H_

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>


#include "render/Camera.h"


namespace seca
{
	namespace viewer
	{
		class WindowCamera
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

			void updateMouse(GLFWwindow *window);


		public:
			WindowCamera();
			~WindowCamera();

			void setWindow(GLFWwindow *_window);

			glm::mat4x4 getMVP();

			void update();

			void OnScroll(double offsetx, double offsety);
			void OnScrollStub(GLFWwindow * window, double offsetx, double offsety);

			void OnMouseButton(GLFWwindow * m_window, int button, int action, int mods);
			void OnMouseButtonStub(GLFWwindow * window, int button, int action, int mods);
		};
	}
}

#endif // SECA_VIEWER_WINDOW_CAMERA_H_
