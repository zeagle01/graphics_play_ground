


#pragma once



#include <unordered_map>
#include <string>

class Shader
{
private:
    unsigned int m_id;

	std::unordered_map<std::string, int> m_uniform_location_cache;

public:
    Shader();
    ~Shader();

    void bind()const ;
    void unbind()const ;

    void set_uniform_4f(const std::string &name, float v0, float v1, float v2, float v3);
	void set_uniform_mat4f(const std::string& name, float* mat4);
    void set_uniform_1i(const std::string &name,int v);

    void create_shader_from_file(const std::string &vertex_shader,const  std::string &fragment_shader);

private:
    int get_uniform_location(const std::string &name);

    void create_shader(std::string &vertex_shader, std::string &fragment_shader);
    std::string read_source_from_file(const std::string &file);
    unsigned int compile_shader(std::string &shader, unsigned int type);
};

