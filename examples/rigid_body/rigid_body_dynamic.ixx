
module;

#include <vector>

export module rigid_body_dynamic;


import quick_shell;
import matrix_math;

using vec3f = matrix_math::matrix<float, 3, 1>;
using vec2f = matrix_math::matrix<float, 2, 1>;
using vec3i = matrix_math::matrix<int, 3, 1>;
using vec2i = matrix_math::matrix<int, 2, 1>;

using mat3f = matrix_math::matrix<float, 3, 3>;

export
class rigid_body_dynamic
{
public:
	void run() 
	{

		m_window.init(800, 600);

		auto& renderer = m_window.get_renderer();
		std::vector<float> pos{
			0.f, 0.f, 0.f,
			1.f, 0.f, 0.f,
			1.f, 1.f, 0.f
		};

		std::vector<int> triangles{ 0, 1, 2 };


		m_window.register_frame_update_fn(
			[&]()
			{
				renderer.draw_triangles(triangles.data(), pos.data(), triangles.size() / 3, pos.size() / 3);
			}
		);

		m_window.run_event_loop();

	}

private:
	quick_shell::main_window m_window;

};