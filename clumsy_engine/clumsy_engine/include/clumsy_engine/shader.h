

#pragma once

#include "glm/glm.hpp"

#include <string>
#include <memory>


namespace clumsy_engine
{

    class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		static std::unique_ptr<Shader> create(const std::string& vertex_src, std::string& fragment_src);
	};

}



