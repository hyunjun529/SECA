#include "Axis.h"


void seca::objects::Axis::createAxisObject()
{
	glm::vec3 v[2];

	m_axis_object.drawHow = GL_LINES;

	v[0] = glm::vec3(0, 0, 0);

	v[1] = glm::vec3(m_length_x, 0, 0);
	m_axis_object.bufferPosition.push_back(v[0]);
	m_axis_object.bufferPosition.push_back(v[1]);

	v[1] = glm::vec3(0, m_length_y, 0);
	m_axis_object.bufferPosition.push_back(v[0]);
	m_axis_object.bufferPosition.push_back(v[1]);

	v[1] = glm::vec3(0, 0, m_length_z);
	m_axis_object.bufferPosition.push_back(v[0]);
	m_axis_object.bufferPosition.push_back(v[1]);
}

const seca::render::Object seca::objects::Axis::getAxisObject()
{
	return m_axis_object;
}