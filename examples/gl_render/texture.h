



#pragma once

#include <string>


class Texture
{
private:
	unsigned int m_id;
	std::string m_file;
	unsigned char* m_local_buffer;
	int m_width;
	int m_height;
	int m_bit_per_pixel;

public:
	Texture(const std::string& file);
	~Texture();

	void bind(unsigned int slot) const;
	void unbind() const;

	inline int  get_width()const ;
	inline int  get_height()const ;
};