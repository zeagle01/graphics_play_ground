
#include "openGL_renderer_API.h"
#include "glad/glad.h"
#include "vertex_array.h"
namespace clumsy_engine
{

	void OpenGL_Renderer_API::init()
	{

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}

	void OpenGL_Renderer_API::clear()
	{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGL_Renderer_API::set_clear_color(const glm::vec4& color)
	{
		glClearColor(color[0], color[1], color[2], color[3]);

	}

	void OpenGL_Renderer_API::draw_indexed(Ref<Vertex_Array> vertex_array)
	{
		glDrawElements(GL_TRIANGLES, vertex_array->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr);

	}

}
