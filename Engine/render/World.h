#ifndef KATA_RENDER_WINDOW_H_
#define KATA_RENDER_WINDOW_H_

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "ShaderUtil.h"

namespace kata
{
	namespace render
	{
		class World
		{
		private:
			ShaderUtil shaderUtil;

			int m_windowWidth = 400;
			int m_windowHeight = 400;

			char windowTitle[64] = "??";

		public:
			GLFWwindow * m_window;

			World(int _w = 400, int _h = 400)
				: m_windowWidth(_w), m_windowHeight(_h)
			{
				m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, windowTitle, NULL, NULL);
			}

			void updateWindowTitle(const char *_title)
			{
				strncpy_s(windowTitle, _title, sizeof(windowTitle));
				glfwSetWindowTitle(m_window, windowTitle);
			}

			const GLchar *loadShader(const char *_path)
			{
				if (strcmp(_path, "UVonly.vert") >= 0) return shaderUtil.vertUVonly;
				if (strcmp(_path, "UVonly.frag") >= 0) return shaderUtil.fragUVonly;
				return "";
			}
		};
	}
}


#endif // KATA_RENDER_WINDOW_H_