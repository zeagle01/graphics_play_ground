
#pragma  once

#include "renderer_API.h"

namespace clumsy_engine
{

	class Render_Command
	{
	public:

		inline static void clear() { s_renderer_API->clear(); }

		inline static void set_clear_color(const glm::vec4& color) { s_renderer_API->set_clear_color(color); }

		static inline void draw_indexed(std::shared_ptr<Vertex_Array> vertex_array)
		{
			s_renderer_API->draw_indexed(vertex_array);
		}

	private:
		static std::shared_ptr<Renderer_API> s_renderer_API;

	};

}