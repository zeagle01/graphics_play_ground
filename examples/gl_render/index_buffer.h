

#pragma  once



class Index_Buffer
{
private:
	unsigned int m_renderer_id;
	unsigned int m_element_num;

public:
	Index_Buffer(const unsigned int* data,unsigned int element_num);
	Index_Buffer() = default;
	virtual ~Index_Buffer();

	void bind()const;
	void unbind()const ;

	void set_data(const unsigned int* data, unsigned int element_num);

	unsigned int get_count()const ;

};