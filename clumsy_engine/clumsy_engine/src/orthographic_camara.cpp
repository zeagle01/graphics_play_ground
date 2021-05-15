
#include "orthographic_camara.h"
#include "glm/gtc/matrix_transform.hpp"

namespace clumsy_engine
{

	void Orthorgraphic_Camara::set_view_field(float left, float right, float bottom, float top)
	{
		m_projection_matrix = glm::ortho(left, right, bottom, top, -1.f, 1.f);
		m_view_matrix = glm::mat4(1.f);
		m_position = glm::vec3(0.f);
		m_rotation = 0.f;
		m_view_projection_matrix = m_projection_matrix * m_view_matrix;
	}

	void Orthorgraphic_Camara::recalculate_view_matrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.f), m_position)
			* glm::rotate(glm::mat4(1.0), m_rotation, glm::vec3(0, 0, 1));

		m_view_matrix = glm::inverse(transform);
		m_view_projection_matrix = m_projection_matrix * m_view_matrix;
	}

	void Orthorgraphic_Camara::set_view_matrix(const glm::mat4& view_matrix)
	{
		m_view_matrix = view_matrix;
		m_view_projection_matrix = m_projection_matrix * m_view_matrix;

	}


	void Orthorgraphic_Camara::set_projection_matrix(const glm::mat4& projection_matrix)
	{
		m_projection_matrix = projection_matrix;
		m_view_projection_matrix = m_projection_matrix * m_view_matrix;

	}

	///////////////////////

//	Orthorgraphic_Camara_NEW::Orthorgraphic_Camara_NEW()
//		:
//		m_view_matrix(glm::mat4(1.f))
//		, m_position(glm::vec3(0.f))
//	{
//
//	}
//
//	void Orthorgraphic_Camara_NEW::set_view_field(float left, float right, float bottom, float top, float near, float far)
//	{
//		//m_projection_matrix = glm::ortho(left, right, bottom, top, -1.f, 1.f);
//
//		m_projection_matrix = glm::ortho(left, right, bottom, top, std::abs(near), std::abs(far)); 
//
//		float aspect_ration=(top - bottom) / (right - left);
//		float fov_degree = std::atan(std::abs(top) / std::abs(near)) * 180.f / 3.1415926f;
//		//m_projection_matrix = glm::perspective(fov_degree, aspect_ration, std::abs(near), std::abs(far));
//		recompute_view_projection_matrix();
//	}
//
//	void Orthorgraphic_Camara_NEW::set_position(const glm::vec3& position)
//	{
//		m_position = position; 
//
//		glm::mat4 transform = glm::translate(glm::mat4(1.f), m_position);
//		m_view_matrix = glm::inverse(transform);
//		recompute_view_projection_matrix();
//	}
//	void Orthorgraphic_Camara_NEW::move_position(const glm::vec3& delta_x)
//	{
//		m_position += delta_x;
//
//		glm::mat4 transform = glm::translate(glm::mat4(1.f), m_position);
//		m_view_matrix = glm::inverse(transform);
//		recompute_view_projection_matrix();
//	}
//
//
//	void Orthorgraphic_Camara_NEW::look_at(const glm::vec3& target_position, const glm::vec3& up)
//	{
//		m_view_matrix = glm::lookAt(m_position, target_position, up);
//		recompute_view_projection_matrix();
//	}
//
//
//	void Orthorgraphic_Camara_NEW::recompute_view_projection_matrix()
//	{
//		m_view_projection_matrix = m_projection_matrix * m_view_matrix;
//	}

}
