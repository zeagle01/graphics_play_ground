
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

		GLenum internal_format = 0, data_format = 0;
		if (channel == 4)
		{
			internal_format = GL_RGBA8;
			data_format = GL_RGBA;
		}
		else if (channel==3)
		{
			internal_format = GL_RGB8;
			data_format = GL_RGB;
		}
		else
		{
			CE_CORE_ERROR("not support image for texture: channel {0}", channel);
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_render_id);
		glTextureStorage2D(m_render_id, 1, internal_format, m_width, m_height);

		glTextureParameteri(m_render_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_render_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_render_id, 0, 0, 0, m_width, m_height, data_format, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
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
}