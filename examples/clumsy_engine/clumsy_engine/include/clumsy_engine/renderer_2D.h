
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
		static void flush();

	public:


		struct param
		{
			const glm::vec2& position;
			const glm::vec2& size = { 1.f, 1.f };
			float rotation = 0.f;
			Ref<Texture_2D> texture = nullptr;
			float tilling_factor = 1.f;
			const glm::vec4& tint_color = glm::vec4(1.f);
		};

		static void draw_quad(param p);

	};

//	//TODO:
//	struct quad_drawer
//	{
//		quad_drawer& with_position(const glm::vec2& p);
//		quad_drawer& with_size(const glm::vec2& s);
//		quad_drawer& with_rotation(float degree);
//		quad_drawer& with_color(float degree);
//		quad_drawer& with_texture(Ref<Texture_2D>);
//	};

}

