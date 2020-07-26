


#pragma once

#include <memory>
#include "layer.h"
#include <vector>


namespace clumsy_engine
{

	class Layer_Stack
	{
	public:
		Layer_Stack();
		~Layer_Stack();


		void push_layer(std::shared_ptr<Layer> layer);
		void push_overlay(std::shared_ptr<Layer> overlay);

		void pop_layer(std::shared_ptr<Layer> layer);
		void pop_overlay(std::shared_ptr<Layer> overlay);

		using  layer_vec = std::vector<std::shared_ptr<Layer>>;
		layer_vec::iterator begin() { return m_layers.begin(); }
		layer_vec::iterator end() { return m_layers.end(); };

	private:
		layer_vec m_layers;
		layer_vec::iterator m_layer_insert;
	};

}
