



#pragma once


#include <vector>

#include "glad/glad.h"
#include "glog/logging.h"


struct Vertex_Buffer_Element
{
	unsigned int count;
	unsigned int  type;
	unsigned char normalize;
	static unsigned int get_size_of_type(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		LOG(ERROR) << "no type ";
		return 0;
	}
};

class Vertex_Buffer_Layout
{
private:
	std::vector<Vertex_Buffer_Element> m_elements;
	unsigned int m_stride;

public:
	Vertex_Buffer_Layout();

	template<typename T>
	void push(unsigned int count)
	{
		//m_elements.push_back({ .count = count,.type = T });
		static_assert(false);
	}

	template<>
	void push<float>(unsigned int count)
	{
		m_elements.push_back({ .count = count,.type = GL_FLOAT ,.normalize = GL_FALSE });
		m_stride += count * Vertex_Buffer_Element::get_size_of_type(GL_FLOAT);
	}

	template<>
	void push<unsigned int >(unsigned int count)
	{
		m_elements.push_back({ .count = count,.type = GL_UNSIGNED_INT,.normalize = GL_FALSE });
		m_stride += count * Vertex_Buffer_Element::get_size_of_type(GL_UNSIGNED_INT);

	}

	template<>
	void push<unsigned char >(unsigned int count)
	{
		m_elements.push_back({ .count = count,.type = GL_UNSIGNED_BYTE,.normalize = GL_TRUE });
		m_stride += count * Vertex_Buffer_Element::get_size_of_type(GL_UNSIGNED_BYTE);
	}


	//	inline 
	inline unsigned int get_stride() const
	{
		return m_stride;
	}

	inline const std::vector<Vertex_Buffer_Element>& get_elements() const  
	{
		return m_elements;
	}


};