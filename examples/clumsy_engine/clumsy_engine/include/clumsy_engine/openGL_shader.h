


#pragma once

#include  "shader.h"
#include "glm/glm.hpp"

#include <string>
#include <unordered_map>


namespace clumsy_engine
{

	class OpenGL_Shader : public Shader
	{
	public:
		OpenGL_Shader(const std::string& name, const std::string& vertex_src, std::string& fragment_src);
		OpenGL_Shader(const std::string& shader_file);

		void bind() const override;
		void unbind() const override;

		const std::string& get_name() const override { return m_name;}

		void upload_uniform_mat4(const std::string& name, const glm::mat4& m) override;
		void upload_uniform_vec3(const std::string& name, const glm::vec3& m) override;
		void upload_uniform_vec4(const std::string& name, const glm::vec4& m) override;
		void upload_uniform_float(const std::string& name, const float& v) override;
		void upload_uniform_int(const std::string& name, const int& v) override;
		void upload_uniform_int_array(const std::string& name, int* v, int count) override;

		unsigned int get_id() override { return m_renderer_id; }
	private:
		void compile(const std::unordered_map<unsigned int, std::string>& shader_sources_by_type);
		std::unordered_map<unsigned int, std::string> split_according_to_shader_type(const std::string& all_in_one_source_code);
		std::string read_shader_file(const std::string& shader_file);

	private:
		unsigned int m_renderer_id;
		std::unordered_map<unsigned int, std::string> m_shader_sources_by_type;
		std::string m_name;
	};

}


