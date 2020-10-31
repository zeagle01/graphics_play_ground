

#pragma once

#include <vector>
#include "signal.h" 

namespace clumsy_engine
{


	class Data_Wrapper
	{
	public:
		virtual  ~Data_Wrapper() {}
	};

	template<typename T>
	class Broad_Cast_Data :public Data_Wrapper
	{

	public:
		using cr_signal = signal<const T&>;
		using cr_slot = slot<const T&>;
		using data_type = T;

		T data;
		cr_signal sig;

		void send_to(cr_slot s)
		{
			sig.connect(s);
		}

		void set(const T& data)
		{
			this->data = data;
			sig(data);
		}

		const T& get()
		{
			return data;
		}
	};

	///
	class Position :public Broad_Cast_Data<std::vector<float>> {};
	class Last_Frame_Position :public Broad_Cast_Data<std::vector<float>> {};
	class Triangle_Indices :public Broad_Cast_Data<std::vector<int>> {};
	///


	class Simulation_Data
	{

	public:

		void set_mesh(const std::vector<float>& positions, const std::vector<int>& triangles)
		{

			m_triangles = triangles;


			int position_size = positions.size();


			m_velocities.resize(position_size);

		}



	public:

//		std::vector<float> m_gravity;
//
//		float m_time_step;

		std::vector<float> m_masses;

		std::vector<float> m_velocities;


		std::vector<int> m_triangles;

		//Broad_Cast_Data<std::vector<float>> m_positions;

	};




}