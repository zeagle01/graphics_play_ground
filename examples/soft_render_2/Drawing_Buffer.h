
#pragma once

#include <functional>
#include <vector>

class Drawing_Buffer
{
public:

	void init(int width, int height);

	void main_loop(std::function<void()>);

	void set_color(int wi, int hi, float r, float g, float b);

	void clear();

private:
	std::vector<uint32_t> buffer;
	int m_width;
	int m_height;


};