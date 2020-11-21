
#include "renderer.h"
#include "render_command.h"
#include "vertex_array.h"

namespace clumsy_engine
{

	void Renderer::begin_scene()
	{

	}
	void Renderer::end_scene()
	{

	}
	void Renderer::submit(std::shared_ptr<Vertex_Array> vertex_array)
	{
		vertex_array->bind();
		Render_Command::draw_indexed(vertex_array);
	}
}