#ifndef SECA_VISUALIZATION_GRID_H_
#define SECA_VISUALIZATION_GRID_H_


#include <GL/gl3w.h>
#include <glm/glm.hpp>


#include "render/Object.h"


namespace seca
{
	namespace visualization
	{
		class Grid
		{
		private:
			glm::vec3 m_pos_center = glm::vec3(0, 0, 0);
			GLint m_cell_row_cnt = 10;
			GLint m_cell_col_cnt = 10;
			GLfloat m_cell_row_size = 1.f;
			GLfloat m_cell_col_size = 1.f;

			render::Object m_grid_object;

		public:
			Grid() {}
			~Grid() {}

			void CreateGridObject();
			render::Object getGridObject();
		};
	}
}

#endif // SECA_VISUALIZATION_GRID_H_
