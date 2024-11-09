
#include "openGL_texture.h"
#include "stb_image.h"
#include "log.h"
#include "glad/glad.h"



namespace clumsy_engine
{

	OpenGL_Texture_2D::OpenGL_Texture_2D(const std::string& path)
	{
		stbi_set_flip_vertically_on_load(1);
		int channel;
		stbi_uc* data = stbi_load(path.c_str(), &m_width, &m_height, &channel, 0);
		if (!data)
		{
			CE_CORE_ERROR("faile to load image {0}",path);
		}

		m_internal_format = 0,m_data_format = 0;
		if (channel == 4)
		{
			m_internal_format = GL_RGBA8;
			m_data_format = GL_RGBA;
		}
		else if (channel==3)
		{
			m_internal_format = GL_RGB8;
			m_data_format = GL_RGB;
		}
		else
		{
			CE_CORE_ERROR("not support image for texture: channel {0}", channel);
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_render_id);
		glTextureStorage2D(m_render_id, 1, m_internal_format, m_width, m_height);

		glTextureParameteri(m_render_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_render_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_render_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGL_Texture_2D::OpenGL_Texture_2D(int width, int height):
		m_width(width), m_height(height)
	{

		m_internal_format = GL_RGBA8;
		m_data_format = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_render_id);
		glTextureStorage2D(m_render_id, 1, m_internal_format, m_width, m_height);

		glTextureParameteri(m_render_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_render_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}

	void OpenGL_Texture_2D::set_data(void* data, int size)
	{
		int bpc = m_data_format == GL_RGBA ? 4 : 3;
		if (size == m_width * m_height * bpc)
		{
			glTextureSubImage2D(m_render_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			CE_CORE_ERROR("size don't match");
		}

	}

	int OpenGL_Texture_2D::get_width()
	{
		return m_width;

	}

	int OpenGL_Texture_2D::get_heigth()
	{
		return m_height;
	}

	void OpenGL_Texture_2D::bind(int slot) const
	{
		glBindTextureUnit(slot, m_render_id);
	}


	OpenGL_Texture_2D::~OpenGL_Texture_2D()
	{
		glDeleteTextures(1, &m_render_id);
		CE_CORE_INFO("texture deleted");
	}

	uint32_t OpenGL_Texture_2D::get_render_id() const
	{
		return m_render_id;

	}
}