


#include "camara.h"
#include "log.h"

namespace clumsy_engine
{

	glm::mat4 View_Handler::compute_view_matrix(const glm::vec3& camara_position, const glm::vec3& target_position, const glm::vec3& camara_up)
	{
		return glm::lookAt(camara_position, target_position, camara_up);
	}

	void View_Handler::translate(glm::vec3& camara_position, glm::vec3& target_position, glm::vec3& camara_up, const glm::vec2& delta_in_screen)
	{
		auto z_axis = glm::normalize(camara_position - target_position);
		auto y_axis = camara_up;
		auto x_axis = glm::normalize(glm::cross(y_axis, z_axis));

		float senstive = 1e-3f;

		camara_position += delta_in_screen[0] * x_axis * senstive;
		camara_position += delta_in_screen[1] * y_axis * senstive;

		target_position += delta_in_screen[0] * x_axis * senstive;
		target_position += delta_in_screen[1] * y_axis * senstive;
	}

	void View_Handler::rotate(glm::vec3& camara_position, glm::vec3& target_position, glm::vec3& camara_up, const glm::vec2& delta_in_screen)
	{
		m_theta += delta_in_screen.y * 1e-3f;
		m_phi += delta_in_screen.x * 1e-3f;

		float r = glm::length(camara_position - target_position);
		camara_position.y = target_position.y + r * std::sin(m_theta);
		camara_position.z = target_position.z + r * std::cos(m_theta) * std::cos(m_phi);
		camara_position.x = target_position.x + r * std::cos(m_theta) * std::sin(m_phi);
		camara_up.y = std::cos(m_theta);
		camara_up.z = -std::sin(m_theta) * std::cos(m_phi);
		camara_up.x = std::sin(m_theta) * std::sin(m_phi);

	}



	////////////////projection

	glm::mat4 Perspective_Projection::compute_projection_matrix(float left, float right, float bottom, float top, float n, float f)
	{
		float aspect_ration = (top - bottom) / (right - left);
		float fov_degree = std::atan(std::abs(top) / std::abs(n)) * 180.f / 3.1415926f;
		return glm::perspective(fov_degree, aspect_ration, std::abs(n), std::abs(f));
	}

	glm::mat4 Perspective_Projection::zoom(float left, float right, float bottom, float top, float n, float f, float delta)
	{
		//TODO:
		return compute_projection_matrix(left, right, bottom, top, n, f);
	}

	glm::mat4 Orthographic_Projection::compute_projection_matrix(float left, float right, float bottom, float top, float n, float f)
	{
		return glm::ortho(left, right, bottom, top, std::abs(n), std::abs(f));
	}
	glm::mat4 Orthographic_Projection::zoom(float left, float right, float bottom, float top, float n, float f, float delta)
	{
		//TODO:
		return compute_projection_matrix(left, right, bottom, top, n, f);
	}









	///////////
	void Camara_Model::set_view_field(float left, float right, float bottom, float top, float n, float f)
	{
		m_projection_matrix = m_projection_handler->compute_projection_matrix(left, right, bottom, top, n, f);
		recompute_view_projection_matrix();
	}

	void Camara_Model::set_look_at(const glm::vec3& camara_position, const glm::vec3& target_position, const glm::vec3& camara_up)
	{
		m_up = camara_up;
		m_target_position = target_position;
		m_position = camara_position;
		m_view_matrix = m_view_handler->compute_view_matrix(camara_position, target_position, camara_up); 
		recompute_view_projection_matrix();
	}

	void Camara_Model::translate(const glm::vec2& delta_in_screen)
	{
		m_view_handler->translate(m_position, m_target_position, m_up, delta_in_screen);
		set_look_at(m_position, m_target_position, m_up);
	}

	void Camara_Model::rotate(const glm::vec2& delta_theta)
	{
		m_view_handler->rotate(m_position, m_target_position, m_up, delta_theta);
		set_look_at(m_position, m_target_position, m_up);
	}

	void Camara_Model::zoom(const float& v)
	{

	}


}