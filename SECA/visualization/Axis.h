#ifndef SECA_VISUALIZATION_AXIS_H_
#define SECA_VISUALIZATION_AXIS_H_


#include <GL/gl3w.h>
#include <glm/glm.hpp>


#include "render/Object.h"


namespace seca
{
	namespace visualization
	{
		class Axis
		{
		private:
			GLfloat m_length_x = 10.f;
			GLfloat m_length_y = 10.f;
			GLfloat m_length_z = 10.f;

			render::Object m_axis_object;

		public:
			Axis() {}
			~Axis() {}

			void createAxisObject();
			const render::Object getAxisObject();
		};
	}
}

#endif // SECA_VISUALIZATION_GRID_H_
