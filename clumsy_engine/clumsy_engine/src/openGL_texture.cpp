
#include "openGL_texture.h"
#include "stb_image.h"



namespace clumsy_engine
{

	OpenGL_Texture_2D::OpenGL_Texture_2D(const std::string& path)
	{

	}

	int OpenGL_Texture_2D::get_width()
	{
		return m_width;

	}

	int OpenGL_Texture_2D::get_heigth()
	{
		return m_height;
	}

	void OpenGL_Texture_2D::bind() const
	{

	}

}