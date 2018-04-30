#ifndef KATA_COMPONENT_SCENECOMPONENT_H_
#define KATA_COMPONENT_SCENECOMPONENT_H_

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "../../render/World.h"

#include "../Component.h"
#include "../InputComponent.h"
#include "../PhysicsComponent.h"
#include "../GraphicsComponent.h"

#include "../InputComponent/ImguiViewerInputComponent.h"


namespace kata
{
	namespace component
	{
		class RenderComponent : public GraphicsComponent
		{
		protected:
			render::World *m_world = nullptr;
			
			GLuint *m_pixel = nullptr;
			
			bool isSingleWindow = false;

			int m_windowSizeW = 400;
			int m_windowSizeH = 400;

		public:
			RenderComponent() {}

			void setGLWindow(render::World *_world, int _w, int _h)
			{
				m_world = _world;
				m_windowSizeW = _w;
				m_windowSizeH = _h;
			}

			void setPixel(GLuint *pixel)
			{
				m_pixel = pixel;
			}

			GLuint* getPixel()
			{
				glReadPixels(0, 0, m_windowSizeW, m_windowSizeH, GL_RGBA, GL_UNSIGNED_INT, m_pixel);
			}

			void onSingleWindowMode()
			{
				isSingleWindow = true;
			}

			virtual void setup() {}

			virtual void render() {}
		};
	}
}

#endif // KATA_COMPONENT_SCENECOMPONENT_H_
