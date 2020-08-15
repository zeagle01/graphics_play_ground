
#ifndef TOPOLOGY_COMPUTER_H 
#define TOPOLOGY_COMPUTER_H 


#include "predef_types.h"
#include <vector>
#include <memory>
#include <functional>


class Topology_Computer {
public:
	//void compute_edge_indices(m2xi& edge_indices,const m3xi& triangle_indices);
	void get_edge_indices(m2xi& edge_indices);
	std::function<std::vector<int>(const int )>  get_one_ring();
	void precompute(const m3xi& triangle_indices);

private:

struct HE_NEW {
	int v=-1;//point to
	int t=-1;//
	int n=-1;
};

struct T_NEW {
	int h=-1;
	int i=-1;
};

struct V_NEW {
	int h=-1;
	int i=-1;

};

	std::vector<HE_NEW> he_list_NEW;
	std::vector<T_NEW> triangle_list_NEW;
	std::vector<V_NEW> vertex_list_NEW;


};


#endif