

#pragma  once



class Vertex_Buffer
{
private:
	unsigned int m_renderer_id;

public:
	Vertex_Buffer(const void* data,unsigned int size);
	Vertex_Buffer() = default;
	virtual ~Vertex_Buffer();

	void bind()const ;
	void unbind()const ;

	void set_data(const void* data, unsigned int size);

};