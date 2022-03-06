
#pragma once

#include "ref.h"
#include "glm/glm.hpp"

namespace clumsy_engine
{

    class Camara;

	class Renderer_2D
	{
	public:
		static void init();
		static void shutdown();

		static void begin_scene(Ref<Camara> camara);
		static void	end_scene();

			//
		static void draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void draw_quad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

	};

}

