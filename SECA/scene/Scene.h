#ifndef KATA_SCENE_SCENE_H_
#define KATA_SCENE_SCENE_H_

#include <malloc.h>

#include "render/World.h"


namespace kata
{
	namespace scene
	{
		class Scene
		{
		protected:
			render::World *m_world = nullptr;

			GLuint *m_pixel;

			int m_sceneSizeW = 400;
			int m_sceneSizeH = 400;

			bool isSingleWindow = false;

		public:
			virtual void setup() {}

			virtual void reset() {}

			void setWorld(render::World *_world)
			{
				if (m_world != nullptr)
				{
					delete(m_world);
				}
				m_world = _world;
			}

			void setSceneSize(int _w, int _h)
			{
				m_sceneSizeW = _w;
				m_sceneSizeH = _h;
			}

			void setSingleMode(bool _flag) { isSingleWindow = _flag; }

			GLuint* getPixel() { return m_pixel; }
		};
	}
}

#endif // KATA_SCENE_SCENE_H_