
#pragma once

#include "renderer_API.h"

namespace clumsy_engine
{


	class OpenGL_Renderer_API :public Renderer_API
	{
	public:

		void init() override;
		void clear() override;
		void set_polygon_mode(bool isLine) override;
		void set_clear_color(const glm::vec4& color) override;
		void set_viewport(int x, int y, int  width, int height) override;

		void draw_indexed(Ref<Vertex_Array> vertex_array) override;
		void draw_indexed(Ref<Vertex_Array> vertex_array, int count) override;

	};

}