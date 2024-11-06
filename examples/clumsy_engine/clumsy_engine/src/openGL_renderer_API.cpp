
#include "openGL_renderer_API.h"
#include "glad/glad.h"
#include "vertex_array.h"
#include "log.h"
namespace clumsy_engine
{

	void OpenGL_Renderer_API::init()
	{

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

	}

	void OpenGL_Renderer_API::clear()
	{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGL_Renderer_API::set_clear_color(const glm::vec4& color)
	{
		glClearColor(color[0], color[1], color[2], color[3]);

	}

	void OpenGL_Renderer_API::set_polygon_mode(bool isLine)
	{
		if (isLine)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	void OpenGL_Renderer_API::draw_indexed(Ref<Vertex_Array> vertex_array)
	{
		glDrawElements(GL_TRIANGLES, vertex_array->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr);

	}

	void OpenGL_Renderer_API::draw_indexed(Ref<Vertex_Array> vertex_array, int count)
	{
		if (count > vertex_array->get_index_buffer()->get_count())
		{
			CE_CORE_WARN("draw count %d exceed vao size %d", count, vertex_array->get_index_buffer()->get_count());
		}

		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

	}

	void OpenGL_Renderer_API::set_viewport(int x, int y, int  width, int height)
	{
		glViewport(x, y, width, height);
	}


}
