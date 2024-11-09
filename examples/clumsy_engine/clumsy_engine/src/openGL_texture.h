
#pragma once

#include "texture.h"
#include <string>
#include "glad/glad.h"
namespace clumsy_engine
{

	class OpenGL_Texture_2D:public Texture_2D
	{
	public:
		OpenGL_Texture_2D(const std::string& path);
		OpenGL_Texture_2D(int width,int height);
		~OpenGL_Texture_2D();


		int get_width() override;
		int get_heigth() override;

		void set_data(void* data, int size) override;

		void bind(int slot) const override;

		uint32_t get_render_id() const override;

	private:
		int m_width;
		int m_height;
		unsigned int m_render_id;

		GLenum m_internal_format = 0;
		GLenum m_data_format = 0;
	};

}
