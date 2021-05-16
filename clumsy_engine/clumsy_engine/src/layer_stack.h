


#pragma once

#include "layer.h"
#include "ref.h"

#include <memory>
#include <vector>


namespace clumsy_engine
{

	class Layer_Stack
	{
	public:
		Layer_Stack();
		~Layer_Stack();


		void push_layer(Ref<Layer> layer);
		void push_overlay(Ref<Layer> overlay);

		void pop_layer(Ref<Layer> layer);
		void pop_overlay(Ref<Layer> overlay);

		using  layer_vec = std::vector<Ref<Layer>>;
		layer_vec::iterator begin() { return m_layers.begin(); }
		layer_vec::iterator end() { return m_layers.end(); };

	private:
		int m_layer_insert_pos = 0;
		layer_vec m_layers;
	};

}
