
#include "openGL_texture.h"
#include "stb_image.h"
#include "log.h"
#include "glad/glad.h"



namespace clumsy_engine
{

	OpenGL_Texture_2D::OpenGL_Texture_2D(const std::string& path)
	{
		stbi_set_flip_vertically_on_load(1);
		int bit_per_pixel;
		stbi_uc* data = stbi_load(path.c_str(), &m_width, &m_height, &bit_per_pixel, 4);//4 for rgba
		if (!data)
		{
			CE_CORE_ERROR("faile to load image {0}",path);
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_render_id);
		glTextureStorage2D(m_render_id, 1, GL_RGB8, m_width, m_height);

		glTextureParameteri(m_render_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_render_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_render_id, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, data);

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