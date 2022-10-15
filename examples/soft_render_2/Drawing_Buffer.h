
#pragma once

#include <functional>
#include <vector>

class Drawing_Buffer
{
public:

	void init(int width, int height);

	void main_loop(std::function<void(uint32_t*)>);

private:
	std::vector<uint32_t> buffer;
};