#include "Grid.h"


#include <GL/gl3w.h>
#include <glm/glm.hpp>


void seca::visualization::Grid::createGridObject()
{
	glm::vec3 v[2];

	m_grid_object.drawHow = GL_LINES;

	GLfloat d_row = -(int)(m_cell_row_cnt / 2) * m_cell_row_size;
	GLfloat u_row = (int)((m_cell_row_cnt + 1) / 2) * m_cell_row_size;
	GLfloat d_col = -(int)(m_cell_col_cnt / 2) * m_cell_col_size;
	GLfloat u_col = (int)((m_cell_col_cnt + 1) / 2) * m_cell_col_size;

	// vertical
	for (int i = 0; i <= m_cell_row_cnt; i++)
	{
		v[0] = glm::vec3(d_row + (i * m_cell_row_size), 0, u_col);
		v[1] = glm::vec3(d_row + (i * m_cell_row_size), 0, d_col);
		m_grid_object.bufferPosition.push_back(v[0]);
		m_grid_object.bufferPosition.push_back(v[1]);
	}

	// horizon
	for (int i = 0; i <= m_cell_row_cnt; i++)
	{
		v[0] = glm::vec3(u_row, 0, d_col + (i * m_cell_col_size));
		v[1] = glm::vec3(d_row, 0, d_col + (i * m_cell_col_size));
		m_grid_object.bufferPosition.push_back(v[0]);
		m_grid_object.bufferPosition.push_back(v[1]);
	}
}

seca::render::Object seca::visualization::Grid::getGridObject()
{
	return m_grid_object;
}