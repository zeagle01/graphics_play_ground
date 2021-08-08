

#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "log.h"
#include <memory>


namespace clumsy_engine
{

	class Camara
	{
	public:
		virtual void set_view_field(float left, float right, float bottom, float top, float n, float f) = 0;
		virtual void set_look_at(const glm::vec3& camara_position, const glm::vec3& target_position, const glm::vec3& camara_up) = 0;

		virtual void move(const glm::vec2& delta_in_screen) {};

		virtual void dragging_handle(const glm::vec2& delta) {}; //remove
		virtual void rotate(const glm::vec2& delta_theta) {};

		virtual void zoom(const float& v) {};

		////////////getter
		virtual const glm::mat4& get_view_projection_matrix() const = 0;
		virtual const glm::vec3& get_position() const = 0;
		virtual const glm::vec3& get_up_direction() const = 0;

	};

	template<typename Dragging_Handler_T, typename Projection_T>
	class Camara_Model :public Camara
	{
	public:
		Camara_Model() :
			m_view_matrix(glm::mat4(1.f))
			, m_position(glm::vec3(0.f)) {}

		void set_view_field(float left, float right, float bottom, float top, float n, float f) override
		{
			m_projection_matrix = m_projection_handler(left, right, bottom, top, n, f);
			recompute_view_projection_matrix();
		}

		void dragging_handle(const glm::vec2& delta)override
		{
			m_dragging_handler(m_position, m_target_position, m_up, delta);
			set_look_at(m_position, m_target_position, m_up);
		}

		void set_look_at(const glm::vec3& camara_position, const glm::vec3& target_position, const glm::vec3& camara_up)override
		{
			m_up = camara_up;
			m_target_position = target_position;
			m_position = camara_position;
			m_view_matrix = glm::lookAt(camara_position, target_position, camara_up);
			recompute_view_projection_matrix();
		}

		////////////getter

		const glm::mat4& get_view_projection_matrix() const override
		{
			return m_view_projection_matrix;
		}

		const glm::vec3& get_position() const override
		{
			return m_position;
		}

		const glm::vec3& get_up_direction() const override
		{
			return m_up;
		}

	private:
		void recompute_view_projection_matrix() { m_view_projection_matrix = m_projection_matrix * m_view_matrix; }
	private:
		glm::mat4 m_projection_matrix;
		glm::mat4 m_view_matrix;
		glm::mat4 m_view_projection_matrix;

		glm::vec3 m_position;
		glm::vec3 m_up;
		glm::vec3 m_target_position;

		Projection_T m_projection_handler;
		Dragging_Handler_T m_dragging_handler;

	};

	struct Orthorgraphic_Projection
	{
		glm::mat4 operator()(float left, float right, float bottom, float top, float n, float f)
		{
			return glm::ortho(left, right, bottom, top, std::abs(n), std::abs(f));
		}
	};

	struct Perspective_Projection
	{
		glm::mat4 operator()(float left, float right, float bottom, float top, float n, float f)
		{
			float aspect_ration = (top - bottom) / (right - left);
			float fov_degree = std::atan(std::abs(top) / std::abs(n)) * 180.f / 3.1415926f;
			return glm::perspective(fov_degree, aspect_ration, std::abs(n), std::abs(f));
		}
	};

	struct Focus_On_Target_Drag
	{
		void operator()(glm::vec3& camara_position, glm::vec3& target_position, glm::vec3& camara_up, const glm::vec2& delta)
		{
			m_theta += delta.y * 1e-3f;
			m_phi += delta.x * 1e-3f;

			float r = glm::length(camara_position - target_position);
			camara_position.y = target_position.y + r * std::sin(m_theta);
			camara_position.z = target_position.z + r * std::cos(m_theta) * std::cos(m_phi);
			camara_position.x = target_position.x + r * std::cos(m_theta) * std::sin(m_phi);
			camara_up.y = std::cos(m_theta);
			camara_up.z = -std::sin(m_theta) * std::cos(m_phi);
			camara_up.x = std::sin(m_theta) * std::sin(m_phi);
		}
		float m_theta = 0.f;
		float m_phi = 0.f;

	};


	class Orthorgraphic_Camara :public Camara_Model<Focus_On_Target_Drag, Orthorgraphic_Projection> { };
	class Perspective_Camara :public Camara_Model<Focus_On_Target_Drag, Perspective_Projection> { };



	////////////////////////////
	class View_Handler
	{
	public:
		virtual glm::mat4 compute_view_matrix(const glm::vec3& camara_position, const glm::vec3& target_position, const glm::vec3& camara_up);
		virtual void move(glm::vec3& camara_position, glm::vec3& target_position, glm::vec3& camara_up, const glm::vec2& delta_in_screen);
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

	class Perspective_Projection_New:public Projection_Handler
	{
	public:
		virtual glm::mat4 compute_projection_matrix(float left, float right, float bottom, float top, float n, float f) override;
		virtual glm::mat4 zoom(float left, float right, float bottom, float top, float n, float f, float delta) override;

	};

	class Orthographic_Projection_New:public Projection_Handler
	{
	public:
		virtual glm::mat4 compute_projection_matrix(float left, float right, float bottom, float top, float n, float f) override;
		virtual glm::mat4 zoom(float left, float right, float bottom, float top, float n, float f, float delta) override;
	};


	class Camara_Model_New : public Camara
	{
	public:
		Camara_Model_New(std::shared_ptr<View_Handler> view_handler,std::shared_ptr<Projection_Handler> projection_handler) :
			m_view_matrix(glm::mat4(1.f))
			, m_position(glm::vec3(0.f))
			, m_view_handler(view_handler)
			, m_projection_handler(projection_handler)
		{}


		void set_look_at(const glm::vec3& camara_position, const glm::vec3& target_position, const glm::vec3& camara_up) override;
		void move(const glm::vec2& delta_in_screen) override;
		void rotate(const glm::vec2& delta_theta) override;
		void dragging_handle(const glm::vec2& delta) override { rotate(delta); } //remove

		void set_view_field(float left, float right, float bottom, float top, float n, float f) override;
		void zoom(const float& v) override;

		////////////getter
		const glm::mat4& get_view_projection_matrix() const override { return m_view_projection_matrix; }
		const glm::vec3& get_position() const override { return m_position; }
		const glm::vec3& get_up_direction() const override { return m_up; }

	private:
		void recompute_view_projection_matrix() { m_view_projection_matrix = m_projection_matrix * m_view_matrix; }

	private:
		glm::mat4 m_projection_matrix;
		glm::mat4 m_view_matrix;
		glm::mat4 m_view_projection_matrix;

		glm::vec3 m_position;
		glm::vec3 m_up;
		glm::vec3 m_target_position;

	private:
		std::shared_ptr<View_Handler> m_view_handler;
		std::shared_ptr<Projection_Handler> m_projection_handler;
	};


	template<typename View_H,typename Projection_H>
	static std::shared_ptr<Camara> new_a_camara() { return std::make_shared< Camara_Model_New > (std::make_shared<View_H>(),std::make_shared<Projection_H>()); };

}
