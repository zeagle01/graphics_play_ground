


#include "OpenGL_Wrapper.h"


void OpenGL_Wrapper::init(int width,int height)
{
	m_width = width;
	m_height = height;

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		printf("glad load failed\n");
	}
	else {

		printf("glad load succend\n");
	}
	printf("gl version %s\n", glGetString(GL_VERSION));

	create_shader();
	make_buffer();
	create_texture();

	int active_texture_slot = 0;

	glBindTextureUnit(active_texture_slot, m_texture_id);
	glBindTexture(GL_TEXTURE_2D, m_texture_id);

	shader.use();
	shader.set_int("u_texture", active_texture_slot);

}

void OpenGL_Wrapper::create_shader()
{
	shader.init();
}

void OpenGL_Wrapper::make_buffer()
{
	glGenVertexArrays(1, (GLuint*)&m_vao);
	glGenBuffers(1, (GLuint*)&m_vbo);
	glGenBuffers(1, (GLuint*)&m_ebo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_positions.size(), m_positions.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_triangles.size(), m_triangles.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void OpenGL_Wrapper::clear()
{
	glClearColor(0., 0.3, 0.5, 1.);//default color;
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGL_Wrapper::draw(void* data)
{

	shader.use();

	glBindTexture(GL_TEXTURE_2D, m_texture_id);
	auto m_data_format = GL_RGBA;
	glTextureSubImage2D(m_texture_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, data);

	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, m_triangles.size(), GL_UNSIGNED_INT, 0);

}

void OpenGL_Wrapper::create_texture()
{

	auto m_internal_format = GL_RGBA8;
	auto m_data_format = GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, (GLuint*) & m_texture_id);
	glTextureStorage2D(m_texture_id, 1, m_internal_format, m_width, m_height);

	glTextureParameteri(m_texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_texture_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//set data
	int size = m_width * m_height;
	std::vector<uint32_t> white_pixel(size);
	for (int i = 0; i < white_pixel.size(); i++)
	{
		white_pixel[i] = 0xffffffff-i;
	}

	int bpc = m_data_format == GL_RGBA ? 4 : 3;

	glTextureSubImage2D(m_texture_id, 0, 0, 0, m_width, m_height, m_data_format, GL_UNSIGNED_BYTE, white_pixel.data());

}
