

#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "log.h"
#include <memory>


namespace clumsy_engine
{

	////////////////////////////
	class View_Handler
	{
	public:
		virtual glm::mat4 compute_view_matrix(const glm::vec3& camara_position, const glm::vec3& target_position, const glm::vec3& camara_up);
		virtual void translate(glm::vec3& camara_position, glm::vec3& target_position, glm::vec3& camara_up, const glm::vec2& delta_in_screen);
		virtual void rotate(glm::vec3& camara_position, glm::vec3& target_position, glm::vec3& camara_up, const glm::vec2& delta_in_screen);
	private:
		float m_theta = 0.f;
		float m_phi = 0.f;
	};

	class Projection_Handler
	{
	public:
		virtual glm::mat4 compute_projection_matrix(float left, float right, float bottom, float top, float n, float f) = 0;
		virtual glm::mat4 zoom(float left, float right, float bottom, float top, float n, float f, float delta) = 0;
	};

	class Perspective_Projection:public Projection_Handler
	{
	public:
		virtual glm::mat4 compute_projection_matrix(float left, float right, float bottom, float top, float n, float f) override;
		virtual glm::mat4 zoom(float left, float right, float bottom, float top, float n, float f, float delta) override;

	private:
		float m_fov_in_degree;
		bool is_init = false;
	};

	class Orthographic_Projection:public Projection_Handler
	{
	public:
		virtual glm::mat4 compute_projection_matrix(float left, float right, float bottom, float top, float n, float f) override;
		virtual glm::mat4 zoom(float left, float right, float bottom, float top, float n, float f, float delta) override;
	};


	class Camara
	{
	public:
		Camara(std::shared_ptr<View_Handler> view_handler,std::shared_ptr<Projection_Handler> projection_handler) :
			m_view_matrix(glm::mat4(1.f))
			, m_position(glm::vec3(0.f))
			, m_view_handler(view_handler)
			, m_projection_handler(projection_handler)
		{}


		virtual void set_look_at(const glm::vec3& camara_position, const glm::vec3& target_position, const glm::vec3& camara_up);
		virtual void translate(const glm::vec2& delta_in_screen);
		virtual void rotate(const glm::vec2& delta_theta);

		virtual void set_view_field(float left, float right, float bottom, float top, float n, float f);
		virtual void zoom(const float& v);

		////////////getter
		const glm::mat4& get_view_projection_matrix() const { return m_view_projection_matrix; }
		const glm::vec3& get_position() const { return m_position; }
		const glm::vec3& get_up_direction() const { return m_up; }

	private:
		void recompute_view_projection_matrix() { m_view_projection_matrix = m_projection_matrix * m_view_matrix; }

	private:
		glm::mat4 m_projection_matrix;
		glm::mat4 m_view_matrix;
		glm::mat4 m_view_projection_matrix;

		float m_left, m_right, m_bottom, m_top, m_near, m_far;

		glm::vec3 m_position;
		glm::vec3 m_up;
		glm::vec3 m_target_position;

	private:
		std::shared_ptr<View_Handler> m_view_handler;
		std::shared_ptr<Projection_Handler> m_projection_handler;
	};


	template<typename View_H,typename Projection_H>
	static std::shared_ptr<Camara> new_a_camara() { return std::make_shared< Camara> (std::make_shared<View_H>(),std::make_shared<Projection_H>()); };

}
