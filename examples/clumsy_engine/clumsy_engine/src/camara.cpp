


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

		float senstive = 1e-2f;

		camara_position += delta_in_screen[0] * x_axis * senstive;
		camara_position += delta_in_screen[1] * y_axis * senstive;

		target_position += delta_in_screen[0] * x_axis * senstive;
		target_position += delta_in_screen[1] * y_axis * senstive;
	}

	void View_Handler::rotate(glm::vec3& camara_position, glm::vec3& target_position, glm::vec3& camara_up, const glm::vec2& delta_in_screen)
	{
		m_theta += delta_in_screen.y * 3e-3f;
		m_phi += delta_in_screen.x * 3e-3f;

		float r = glm::length(camara_position - target_position);
		camara_position.y = target_position.y + r * std::sin(m_theta);
		camara_position.z = target_position.z + r * std::cos(m_theta) * std::cos(m_phi);
		camara_position.x = target_position.x + r * std::cos(m_theta) * std::sin(m_phi);
		camara_up.y = std::cos(m_theta);
		camara_up.z = -std::sin(m_theta) * std::cos(m_phi);
		camara_up.x = std::sin(m_theta) * std::sin(m_phi);

	}



	//////////////// perspective projection

	glm::mat4 Perspective_Projection::compute_projection_matrix(float left, float right, float bottom, float top, float n, float f)
	{
		float aspect_ration = 1.f * (right - left) / (top - bottom);
		float fov = std::atan2(std::abs(top), std::abs(n));
		return glm::perspective(fov, aspect_ration, std::abs(n), std::abs(f));
	}

	glm::mat4 Perspective_Projection::zoom(float left, float right, float bottom, float top, float n, float f, float zoom_level)
	{
		float aspect_ration = 1.f * (right - left) / (top - bottom);
		float fov = std::atan2(std::abs(top), std::abs(n));
		//if (!is_init)
		//{
		//	m_fov_in_degree = glm::degrees(fov);
		//	is_init = true;
		//}
		printf("%f \n", zoom_level);

		float zoomed_fov = glm::degrees(fov) + zoom_level * 2.f;
		float tolerance = 1e-3f;
		zoomed_fov = std::clamp(zoomed_fov, tolerance, 180.f - tolerance);
		return glm::perspective(glm::radians(zoomed_fov), aspect_ration, std::abs(n), std::abs(f));
	}


	//////////////// orthogonal projection
	glm::mat4 Orthographic_Projection::compute_projection_matrix(float left, float right, float bottom, float top, float n, float f)
	{
		return glm::ortho(left, right, bottom, top, std::abs(n), std::abs(f));
	}

	glm::mat4 Orthographic_Projection::zoom(float left, float right, float bottom, float top, float n, float f, float zoom_level)
	{
		return compute_projection_matrix(left * zoom_level, right * zoom_level, bottom * zoom_level, top * zoom_level, n, f);
	}









	///////////
	void Camara::set_view_field(float left, float right, float bottom, float top, float n, float f)
	{

		m_left = left;
		m_right = right;
		m_bottom = bottom;
		m_top = top;
		m_near = n;
		m_far = f;
		m_projection_matrix = m_projection_handler->compute_projection_matrix(left, right, bottom, top, n, f);
		recompute_view_projection_matrix();
	}

	void Camara::set_look_at(const glm::vec3& camara_position, const glm::vec3& target_position, const glm::vec3& camara_up)
	{
		m_up = camara_up;
		m_target_position = target_position;
		m_position = camara_position;
		m_view_matrix = m_view_handler->compute_view_matrix(camara_position, target_position, camara_up); 
		recompute_view_projection_matrix();
	}

	void Camara::translate(const glm::vec2& delta_in_screen)
	{
		m_view_handler->translate(m_position, m_target_position, m_up, delta_in_screen);
		set_look_at(m_position, m_target_position, m_up);
	}

	void Camara::rotate(const glm::vec2& delta_theta)
	{
		m_view_handler->rotate(m_position, m_target_position, m_up, delta_theta);
		set_look_at(m_position, m_target_position, m_up);
	}

	void Camara::zoom(const float& zoom_level)
	{
		m_projection_matrix = m_projection_handler->zoom(m_left, m_right, m_bottom, m_top, m_near, m_far, zoom_level);
		recompute_view_projection_matrix();
	}

}