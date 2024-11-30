

#pragma once

#include "frame_buffer.h"

namespace clumsy_engine
{
	class  openGL_frame_buffer : public frame_buffer
	{
	public:
		openGL_frame_buffer(const frame_buffer::specification& specs);
		~openGL_frame_buffer();

		const frame_buffer::specification& get_specification() const override;

		void bind() override;
		void unbind() override;
		uint32_t get_color_attatchment_render_id() const override;

		void resize();

	private:
		uint32_t m_render_id;
		uint32_t m_color_attatchment;
		uint32_t m_depth_attatchment;
		frame_buffer::specification m_specs;
	};
}
