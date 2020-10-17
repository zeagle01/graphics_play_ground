

#pragma once



#include <vector>
#include <memory>

//#include "interaction.h"

namespace clumsy_engine
{

	class Interaction;

	class Simulator
	{

		std::vector<float> m_delta_positions = {
				0,-0.1,0,
				0,-0.1,0,
				0,-0.1,0
		};

		std::vector<float> m_edge_rest_length = {
			1.f,
			1.f,
			1.f
		};

	public:

		void add_interaction(std::unique_ptr<Interaction> interaction);

		void set_time_step(float h) {}

		void update() {}

		const std::vector<float>& get_delta_positions()const;

	private:

		//std::vector<std::unique_ptr<Interation>> m_interations;

	};

}



