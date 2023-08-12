

#include "layer.h"
#include "log.h"



namespace clumsy_engine
{
	Layer::Layer(const std::string& name) 
		:m_name(name)
	{
		CE_TRACE("Layer {0} {1} created", m_name,(void*)this);
	}

	Layer::~Layer()
	{
		CE_TRACE("Layer {0} {1} destory", m_name,(void*)this);
	}
}