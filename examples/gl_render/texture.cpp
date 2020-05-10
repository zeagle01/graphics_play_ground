
#include "texture.h"
#include "renderer.h"
#include "glad/glad.h"

#include "stb_image.h"

Texture::Texture(const std::string& file):
	m_file(file),
	m_id(0),
	m_local_buffer(nullptr),
	m_width(0),
	m_height(0),
	m_bit_per_pixel(0)
{

	stbi_set_flip_vertically_on_load(1);
	m_local_buffer = stbi_load(file.c_str(), &m_width, &m_height, &m_bit_per_pixel,4);//4 for rgba
	
	GL_Call(glGenTextures(1, &m_id));
	GL_Call(glBindTexture(GL_TEXTURE_2D,m_id));

	// the following 4 is a must
	GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_Call(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GL_Call(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_local_buffer));

	if (m_local_buffer)
	{
		stbi_image_free(m_local_buffer);
	}
}


Texture::~Texture()
{
	GL_Call(glDeleteTextures(1, &m_id));
}

void Texture::bind(unsigned int slot) const
{

	GL_Call(glActiveTexture(GL_TEXTURE0 + slot));
	GL_Call(glBindTexture(GL_TEXTURE_2D, m_id));
}
void Texture::unbind() const
{

	GL_Call(glBindTexture(GL_TEXTURE_2D, 0));
}

inline int  Texture::get_width()const
{

}
inline int  Texture::get_height()const
{

}
