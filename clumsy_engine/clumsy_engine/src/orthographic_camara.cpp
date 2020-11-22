
#include "orthographic_camara.h"
#include "glm/gtc/matrix_transform.hpp"

namespace clumsy_engine
{
	Orthorgraphic_Camara::Orthorgraphic_Camara(float left, float right, float bottom, float top)
		:
		m_projection_matrix(glm::ortho(left,right,bottom,top,-1.f,1.f))
		,m_view_matrix(1.f)
		, m_position(0)
		, m_rotation(0)
	{
		m_view_projection_matrix = m_projection_matrix * m_view_matrix;
	}

	void Orthorgraphic_Camara::recalculate_view_matrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), m_position)
			* glm::rotate(glm::mat4(1.0), m_rotation, glm::vec3(0, 0, 1));

		m_view_matrix = glm::inverse(transform);
		m_view_projection_matrix = m_projection_matrix * m_view_matrix;
	}

}
