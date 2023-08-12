

#pragma once

#include "ref.h"


#include "glm/glm.hpp"

#include <string>
#include <memory>
#include <unordered_map>


namespace clumsy_engine
{

    class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual const std::string& get_name() const = 0;

		virtual void upload_uniform_mat4(const std::string& name, const glm::mat4& m) = 0;
		virtual void upload_uniform_vec3(const std::string& name, const glm::vec3& m) = 0;
		virtual void upload_uniform_vec4(const std::string& name, const glm::vec4& m) = 0;
		virtual void upload_uniform_float(const std::string& name, const float& v) = 0;
		virtual void upload_uniform_int(const std::string& name, const int& v) = 0;

		virtual unsigned int get_id() = 0;

		static std::unique_ptr<Shader> create(const std::string& name, const std::string& vertex_src, std::string& fragment_src);
		static std::unique_ptr<Shader> create(const std::string& shader_file);
	};


	class Shader_Library
	{
	public:
		void add(const Ref<Shader> shader);
		void add(const std::string& name, const Ref<Shader> shader);
		Ref<Shader> load(const std::string& file);
		Ref<Shader> load(const std::string& name, const std::string& file);

		Ref<Shader> get(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_shaders;
	};

}



