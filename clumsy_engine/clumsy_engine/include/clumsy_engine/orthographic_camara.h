

#pragma once

#include "glm/glm.hpp"


namespace clumsy_engine
{
	class Orthorgraphic_Camara
	{
	public:
		Orthorgraphic_Camara(float left, float right, float bottom, float top);

		void set_position(const glm::vec3& position) { m_position = position; recalculate_view_matrix();}
		void set_rotation(float rotation) { m_rotation = rotation; recalculate_view_matrix();}

		void set_view_matrix(const glm::mat4& view_matrix);
		void set_projection_matrix(const glm::mat4& projection_matrix);


		const glm::vec3& get_position()const { return m_position;  }
		float get_rotation()const { return m_rotation;  }

		const glm::mat4& get_projection_matrix() const { return m_projection_matrix; }
		const glm::mat4& get_view_matrix() const { return m_view_matrix; }
		const glm::mat4& get_view_projection_matrix() const { return m_view_projection_matrix; }

	private:
		void recalculate_view_matrix();
	private:

		glm::mat4 m_projection_matrix;
		glm::mat4 m_view_matrix;
		glm::mat4 m_view_projection_matrix;

		glm::vec3 m_position;

		float m_rotation;

	};

}
