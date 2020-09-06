

#include "layer_stack.h"


namespace clumsy_engine
{

	Layer_Stack::Layer_Stack()
	{

	}
	Layer_Stack::~Layer_Stack()
	{
		m_layers.clear();
	}

	void Layer_Stack::push_layer(std::shared_ptr<Layer> layer)
	{
		 m_layers.emplace(m_layers.begin() + m_layer_insert_pos, layer);
		 m_layer_insert_pos++;
	}
	void Layer_Stack::push_overlay(std::shared_ptr<Layer> overlay)
	{
		m_layers.emplace_back(overlay);

	}

	void Layer_Stack::pop_layer(std::shared_ptr<Layer> layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
			m_layer_insert_pos--;
		}

	}
	void Layer_Stack::pop_overlay(std::shared_ptr<Layer> overlay)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
		}
	}

}