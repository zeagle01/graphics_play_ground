

#include "openGL_frame_buffer.h" 

#include "glad/glad.h"

#include "log.h"

namespace clumsy_engine
{
	openGL_frame_buffer::openGL_frame_buffer(const frame_buffer::specification& specs):
		m_specs(specs)
	{
		resize();
	}

	openGL_frame_buffer::~openGL_frame_buffer()
	{
		delete_resource();
	}

	void openGL_frame_buffer::resize()
	{
		glCreateFramebuffers(1, &m_render_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_render_id);

		//color
		glCreateTextures(GL_TEXTURE_2D, 1, &m_color_attatchment);
		glBindTexture(GL_TEXTURE_2D, m_color_attatchment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_specs.width, m_specs.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//attatch color 
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_attatchment, 0);

		//depth
		glCreateTextures(GL_TEXTURE_2D, 1, &m_depth_attatchment);
		glBindTexture(GL_TEXTURE_2D, m_depth_attatchment);
		glTextureStorage2D(m_depth_attatchment, 1, GL_DEPTH24_STENCIL8, m_specs.width, m_specs.height);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_specs.width, m_specs.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
		//attatch depth
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth_attatchment, 0);


		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			CE_CORE_ERROR("wrong frame buffer config !");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	const frame_buffer::specification& openGL_frame_buffer::get_specification() const
	{
		return m_specs;
	}

	void openGL_frame_buffer::bind()
	{

		glBindFramebuffer(GL_FRAMEBUFFER, m_render_id);

		glViewport(0, 0, m_specs.width, m_specs.height);
	}

	void openGL_frame_buffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	uint32_t openGL_frame_buffer::get_color_attatchment_render_id() const
	{
		return m_color_attatchment;
	}

	void openGL_frame_buffer::resize(uint32_t width, uint32_t height) 
	{
		delete_resource();

		m_specs.width = width;
		m_specs.height = height;
		resize();
	}

	void openGL_frame_buffer::delete_resource()
	{
		glDeleteFramebuffers(1, &m_render_id);
		glDeleteTextures(1, &m_color_attatchment);
		glDeleteTextures(1, &m_depth_attatchment);

	}
}

