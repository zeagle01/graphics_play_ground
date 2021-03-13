


template<size_t Row,size_t Col,typename T>
class mat
{
public:
	mat(){}




private:
	static constexpr size_t row = Row;
	static constexpr size_t col = Col;
private:
	T data[row * col];
};