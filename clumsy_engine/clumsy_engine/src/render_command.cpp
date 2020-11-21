

#include "render_command.h"
#include "openGL_renderer_API.h"


namespace clumsy_engine
{

	std::shared_ptr<Renderer_API> Render_Command::s_renderer_API = std::make_shared<OpenGL_Renderer_API>();

}