#pragma once

#include "glm/glm.hpp"
#include <memory>


namespace clumsy_engine
{

	class Vertex_Array;


	class Renderer_API
	{
	public:
		enum class API
		{
			None,
			OpenGL
		};

	public:
		virtual void clear() = 0;
		virtual void set_clear_color(const glm::vec4& color ) = 0;

		virtual void draw_indexed(std::shared_ptr<Vertex_Array> vertex_array) = 0;

		inline static API get_API() { return s_API; };
	private:
		static API s_API;
	};

}