

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
		void resize(uint32_t width, uint32_t height) override;
		uint32_t get_color_attatchment_render_id() const override;


	private:
		void resize();
		void delete_resource();
	private:
		uint32_t m_render_id;
		uint32_t m_color_attatchment;
		uint32_t m_depth_attatchment;
		frame_buffer::specification m_specs;
	};
}
