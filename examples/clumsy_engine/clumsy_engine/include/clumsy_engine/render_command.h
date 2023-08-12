
#pragma  once

#include "renderer_API.h"
#include "clumsy_engine_export.h"

namespace clumsy_engine
{

	class CLUMSY_ENGINE_EXPORT Render_Command
	{
	public:

		inline static void init() { s_renderer_API->init(); }

		inline static void clear() { s_renderer_API->clear(); }

		inline static void set_clear_color(const glm::vec4& color) { s_renderer_API->set_clear_color(color); }

		inline static void set_polygon_mode(bool v) { s_renderer_API->set_polygon_mode(v); }

		inline static void set_viewport(int x, int y, int  width, int height)
		{
			s_renderer_API->set_viewport(x, y, width, height);
		}

		static inline void draw_indexed(Ref<Vertex_Array> vertex_array)
		{
			s_renderer_API->draw_indexed(vertex_array);
		}

	private:
		static Ref<Renderer_API> s_renderer_API;

	};

}