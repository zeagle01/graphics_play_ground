
#pragma once

#include "texture.h"
#include <string>
namespace clumsy_engine
{

	class OpenGL_Texture_2D:public Texture_2D
	{
	public:
		OpenGL_Texture_2D(const std::string& path);
		~OpenGL_Texture_2D();


		int get_width() override;
		int get_heigth() override;
		void bind(int slot) const override;
	private:
		int m_width;
		int m_height;
		unsigned int m_render_id;
	};

}
