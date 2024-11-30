

#pragma once

#include "ref.h"

namespace clumsy_engine
{
	class frame_buffer
	{
	public:
		struct specification
		{
			uint32_t width, height;
			uint32_t samples = 1;
			bool swap_chain_target = false;
		};

	public:
		virtual const specification& get_specification() const = 0;
		virtual void bind() = 0;
		virtual void unbind() = 0;
		virtual uint32_t get_color_attatchment_render_id() const = 0;

		static Ref<frame_buffer> create(const specification& specs);
	};
}
