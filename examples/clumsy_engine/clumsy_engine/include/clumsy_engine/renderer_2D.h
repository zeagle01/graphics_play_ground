
#pragma once

#include "ref.h"
#include "glm/glm.hpp"

namespace clumsy_engine
{

    class Camara;
    class Texture_2D;

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

		static void draw_quad(const glm::vec2& position, const glm::vec2& size, Ref<Texture_2D> texture, float tilling_factor = 1.f, const glm::vec4& tint_color = glm::vec4(1.f));
		static void draw_quad(const glm::vec3& position, const glm::vec2& size, Ref<Texture_2D> texture, float tilling_factor = 1.f, const glm::vec4& tint_color = glm::vec4(1.f));

		static void draw_rotated_quad(const glm::vec3& position, const glm::vec2& size, float rotation, Ref<Texture_2D> texture, float tilling_factor = 1.f, const glm::vec4& tint_color = glm::vec4(1.f));
		static void draw_rotated_quad(const glm::vec2& position, const glm::vec2& size, float rotation, Ref<Texture_2D> texture, float tilling_factor = 1.f, const glm::vec4& tint_color = glm::vec4(1.f));

		static void draw_rotated_quad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void draw_rotated_quad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);

	};

}

