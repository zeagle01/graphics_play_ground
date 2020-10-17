

#pragma once



#include <vector>
#include <memory>


namespace clumsy_engine
{

	class Interaction;
	class System_Equations_Solver;

	class Simulator
	{

		std::vector<float> m_positions0;

		std::vector<float> m_velocity;

		std::vector<float> m_positions;

	public:
		void set_mesh(std::vector<float> positions, std::vector<int> triangles)
		{
			m_positions = positions;
		}

		void add_interaction(std::unique_ptr<Interaction> interaction);

		void set_time_step(float h) { m_time_step = h; }

		void update();

		std::vector<float> get_delta_positions() ;

	private:

		std::vector<std::unique_ptr<Interaction>> m_interations;

		float m_time_step;

		//std::unique_ptr<System_Equations_Solver> m_solver;

	};

}



