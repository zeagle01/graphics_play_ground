
#pragma once

#include "texture.h"
#include <string>
namespace clumsy_engine
{

	class OpenGL_Texture_2D:public Texture_2D
	{
	public:
		OpenGL_Texture_2D(const std::string& path);

		int get_width() override;
		int get_heigth() override;
		void bind() const override;
	private:
		int m_width;
		int m_height;
		int render_id;
	};

}
