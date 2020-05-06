

#pragma  once



class Vertex_Buffer
{
private:
	unsigned int m_renderer_id;

public:
	Vertex_Buffer(const void* data,unsigned int size);
	virtual ~Vertex_Buffer();

	void bind();
	void unbind();

	void set_data(const void* data, unsigned int size);

};