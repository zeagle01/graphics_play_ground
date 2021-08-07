
#pragma once

#include "ref.h"
#include <string>

namespace clumsy_engine
{

	class Texture
	{
	public:
		~Texture() = default;
		virtual int get_width() = 0;
		virtual int get_heigth() = 0;

		virtual void bind(int slot = 0) const = 0;
	};

	class Texture_2D :public Texture
	{
	public:
		static Ref<Texture_2D> create(const std::string& path);

	};

}