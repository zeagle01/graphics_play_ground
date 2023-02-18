
#include "Camara.h"
#include <iostream>

namespace soft_render
{

	Camara::Camara()
	{
		type_map::fill_types<config>(m_configs);
	}


	void Camara::on_press(int button)
	{
		pressed = true;
	}

	void Camara::on_release(int button)
	{
		pressed = false;
		isCursorAssigned = false;
	}

	void Camara::update(int x, int y)
	{
		if (pressed)
		{
			if (isCursorAssigned)
			{
				int dx = x - this->x;
				int dy = y - this->y;
				rotate(dx, dy);
			}
			this->x = x;
			this->y = y;
			isCursorAssigned = true;
		}
	}

	void Camara::rotate(float dx, float dy)
	{

		auto& camara_position = m_configs.get_ref<config::location>();
		auto& camara_up = m_configs.get_ref<config::up>();
		auto target_position = m_configs.get_ref<config::lookat>();

		m_theta += dy * 1e-3f;
		m_phi += -dx * 1e-3f;

		float r = length(camara_position - target_position);
		camara_position(1) = target_position(1) + r * std::sin(m_theta);
		camara_position(2) = target_position(2) + r * std::cos(m_theta) * std::cos(m_phi);
		camara_position(0) = target_position(0) + r * std::cos(m_theta) * std::sin(m_phi);
		camara_up(1) = std::cos(m_theta);
		camara_up(2) = -std::sin(m_theta) * std::cos(m_phi);
		camara_up(0) = std::sin(m_theta) * std::sin(m_phi);

	}
}
