
#pragma once



#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"


class OpenGL_Wrapper
{
public:
	void init(int width, int height);

	void clear();

	void draw(void* data);

private:
	void load_glad();

	void create_texture();
	void bind_texture();

	void create_shader();
	void make_buffer();

	int m_vbo, m_ebo, m_vao;

	std::vector<float> m_positions = {
		-1.f,-1.f,0, 0.f,0.f,
		1.f,-1.f,0,  1.f,0.f,
		1.f,1.f,0,	 1.f,1.f,
		-1.f,1.f,0,	 0.f,1.f
	};

	std::vector<int> m_triangles{
		0,1,2,
		0,2,3
	};

	int m_width ;
	int m_height ;
	int m_texture_id;

	int shader_program;

};
