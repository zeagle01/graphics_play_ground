
#include "Topology_Computer.h"

#include <vector>


struct HE {
	int v=-1;//point to
	int t=-1;//
	int n=-1;
};

struct T {
	int h=-1;
	int i=-1;
};

struct V {
	int h=-1;
	int i=-1;

};

//template<typename T>
//struct Linked_Node {
//	Linked_Node() {
//	}
//
//	Linked_Node(T* data):data(data) {
//	}
//	Linked_Node* next;
//	Linked_Node* pre;
//	T* data;
//};
//
//template<typename T>
//struct Linked_List {
//	using NodeT = Linked_Node<T>;
//	Linked_List() {
//		nil = new NodeT(); 
//		nil->next = nil;
//		nil->pre = nil;
//	}
//	NodeT* nil;
//	void insert_front(NodeT* node) {
//
//		node->next = nil->next;
//		nil->next->pre = node;
//		nil->next = node;
//		node->pre = nil;
//	}
//	void delete_node(NodeT* node) {
//		node->next->pre = node->pre;
//		node->pre->next = node->next;
//	}
//	NodeT* search_node(T* data) {
//		NodeT* x = nil->next;
//		while (x != nil && x->data != data) {
//			x = x->next;
//		}
//		return x;
//	}
//};


void Topology_Computer::compute_edge_indices(m2xi& edge_indices, const m3xi& triangle_indices) {

	int t_num = triangle_indices.cols();
	int he_size = t_num * 6;
	std::vector<std::vector<int>> vv_incidents(he_size);
	std::vector<std::vector<int>> vv_he_pos(he_size);
	std::vector<HE> he_list(he_size);
	std::vector<T> triangle_list(t_num);
	std::vector<V> vertex_list(he_size);


	int he_cur_size=0;
	for (int i = 0; i < t_num; i++) {
		v3i t = triangle_indices.col(i);
		std::vector<HE> t_he(3);
		std::vector<int> t_he_pos(3);
		for (int j = 0; j < 3; j++) {
			int j_next = (j + 1) % 3;
			int v[] = { t[j] ,t[j_next]};
			//new he
			HE& he = t_he[j];
			he.v = v[1];
			he.t = i;


			std::vector<int> v1_incidents = vv_incidents[v[1]];
			auto it=std::find(v1_incidents.begin(), v1_incidents.end(), v[0]);
			int offset = it - v1_incidents.begin();
			int& he_pos = t_he_pos[j];
			if (offset!=v1_incidents.size()) {
				int oppo_he_pos=vv_he_pos[v[1]][offset];
				he_pos = oppo_he_pos + 1;
			}
			else {
				he_pos = he_cur_size;

				HE oppo_he;
				oppo_he.v = v[0];
				int oppo_he_pos = he_pos + 1;

				he_list[oppo_he_pos] = oppo_he;
				vv_he_pos[v[1]].push_back(oppo_he_pos);
				vv_incidents[v[1]].push_back(v[0]);

				he_cur_size += 2;
			}
			he_list[he_pos] = he;
			vv_he_pos[v[0]].push_back(he_pos);
			vv_incidents[v[0]].push_back(v[1]);


			if (vertex_list[v[0]].h == -1) {
				vertex_list[v[0]].h = he_pos;
				vertex_list[v[0]].i = v[0];
			}
		}
		for (int j = 0; j < 3; j++) {
			int j_next = (j + 1) % 3;
			t_he[j].n = t_he_pos[j_next];
		}

		triangle_list[i].h = t_he_pos[0];
		triangle_list[i].i = i;
	}

	int e_num=he_cur_size / 2;
	edge_indices.resize(2,e_num);
	for (int i = 0; i < e_num; i++) {
		edge_indices(0,i) = he_list[i*2+0].v;
		edge_indices(1,i) = he_list[i*2+1].v;
	}
}