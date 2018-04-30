#ifndef KATA_SCENE_OBJVIWERSCENE_H_
#define KATA_SCENE_OBJVIWERSCENE_H_

#include "Scene.h"

#include "component/InputComponent/CameraInputComponent.h"
#include "component/InputComponent/ImguiViewerInputComponent.h"

#include "component/RenderComponenet/OBJRenderComponent.h"


namespace kata
{
	namespace scene
	{
		class OBJViewerScene : public Scene
		{
		private:
			component::CameraInputComponent *m_inputCamera;
			component::ImguiInputComponent *m_inputImgui;
			component::PhysicsComponent *m_physics;
			component::OBJRenderComponent *m_renderOBJ;

		public:
			OBJViewerScene(
				  component::CameraInputComponent *_inputCamera,
				  component::ImguiInputComponent *_inputImgui,
				  component::PhysicsComponent *_phyiscs,
				  component::OBJRenderComponent *_graphics)
				: m_inputCamera(_inputCamera),
				  m_inputImgui(_inputImgui),
				  m_physics(_phyiscs),
				  m_renderOBJ(_graphics)
			{
			}

			void setup()
			{
				if(m_world == nullptr && !isSingleWindow){
					m_world = new render::World();
				}

				m_pixel = (GLuint*)malloc(m_sceneSizeW * m_sceneSizeH * 4 * sizeof(GLuint));

				m_inputCamera->setWindow(m_world->m_window);

				if (isSingleWindow)
				{
					m_renderOBJ->onSingleWindowMode();
				}
				m_renderOBJ->setImguiInputComponent(m_inputImgui);
				m_renderOBJ->setGLWindow(m_world, m_sceneSizeW, m_sceneSizeH);
				m_renderOBJ->setPixel(m_pixel);

				m_renderOBJ->setup();
			}
		};
	}
}

#endif // KATA_SCENE_OBJVIWERSCENE_H_