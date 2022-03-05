#pragma once

#include "glm/glm.hpp"
#include "clumsy_engine_export.h"
#include "ref.h"

#include <memory>


namespace clumsy_engine
{

	class Vertex_Array;


	class CLUMSY_ENGINE_EXPORT Renderer_API
	{
	public:
		enum class API
		{
			None,
			OpenGL
		};

	public:
		virtual void init() = 0;
		virtual void clear() = 0;
		virtual void set_clear_color(const glm::vec4& color ) = 0;

		virtual void draw_indexed(Ref<Vertex_Array> vertex_array) = 0;
		virtual void set_viewport(int x, int y, int  width, int height) = 0;

		inline static API get_API() { return s_API; };
	private:
		static API s_API;
	};

}