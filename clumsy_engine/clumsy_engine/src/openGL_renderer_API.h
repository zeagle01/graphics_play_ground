
#pragma once

#include "renderer_API.h"

namespace clumsy_engine
{


	class OpenGL_Renderer_API :public Renderer_API
	{
	public:

		void clear() override;
		void set_clear_color(const glm::vec4& color) override;

		void draw_indexed(std::shared_ptr<Vertex_Array> vertex_array) override;

	};

}