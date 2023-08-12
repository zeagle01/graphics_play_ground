


#include "shader.h"
#include "openGL_shader.h"
#include "renderer.h"
#include "log.h"
#include "openGL_vertex_array.h"
#include "profiler.h"


namespace clumsy_engine
{

	std::unique_ptr<Shader> Shader::create(const std::string& name, const std::string& vertex_src, std::string& fragment_src)
	{
		switch (Renderer::get_API())
		{
		case Renderer_API::API::None: CE_CORE_WARN("rederer api none!"); return nullptr;
		case Renderer_API::API::OpenGL:  return std::make_unique<OpenGL_Shader>(name, vertex_src, fragment_src);
		}

		CE_CORE_ERROR("Shader create error!");
		return nullptr;
	}

	std::unique_ptr<Shader> Shader::create(const std::string& shader_file)
	{
		switch (Renderer::get_API())
		{
		case Renderer_API::API::None: CE_CORE_WARN("rederer api none!"); return nullptr;
		case Renderer_API::API::OpenGL:  return std::make_unique<OpenGL_Shader>(shader_file);
		}

		CE_CORE_ERROR("Shader create error!");
		return nullptr;
	}


	void Shader_Library::add(const std::string& name, const Ref<Shader> shader)
	{
		if (m_shaders.count(name))
		{
			CE_WARN("shader {0} has already exsits", name);
			return;
		}
		m_shaders[name] = shader;
	}

	void Shader_Library::add(const Ref<Shader> shader)
	{
		const auto& name = shader->get_name();
		add(name, shader);
	}


	Ref<Shader> Shader_Library::load(const std::string& file)
	{
		auto shader = Shader::create(file);
		auto ret = Ref<Shader>(shader.release());
		add(ret);
		return ret;
	}
	Ref<Shader> Shader_Library::load(const std::string& name, const std::string& file)
	{
		auto shader = Shader::create(file);
		auto ret = Ref<Shader>(shader.release());
		add(name, ret);
		return ret;
	}

	Ref<Shader> Shader_Library::get(const std::string& name) const
	{

		if (m_shaders.count(name))
		{
			return m_shaders.at(name);
		}
		else
		{
			CE_ERROR("shader {0} do not exists", name);
			return nullptr;
		}
	}


}  