

#include "layer_stack.h"
#include "log.h"


namespace clumsy_engine
{

	Layer_Stack::Layer_Stack()
	{
		CE_CORE_INFO("create layer_stack {0}", (void*)this);
	}
	Layer_Stack::~Layer_Stack()
	{
		m_layers.clear();

		CE_CORE_INFO("destroy layer_stack {0}", (void*)this);
	}

	void Layer_Stack::push_layer(Ref<Layer> layer)
	{
		 m_layers.emplace(m_layers.begin() + m_layer_insert_pos, layer);
		 m_layer_insert_pos++;
	}
	void Layer_Stack::push_overlay(Ref<Layer> overlay)
	{
		m_layers.emplace_back(overlay);

	}

	void Layer_Stack::pop_layer(Ref<Layer> layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
			m_layer_insert_pos--;
		}

	}
	void Layer_Stack::pop_overlay(Ref<Layer> overlay)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
		}
	}

}