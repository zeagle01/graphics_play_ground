
#include "Topology_Computer.h"

#include <vector>
#include "predef_types.h"

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




void Topology_Computer::precompute(const m3xi& triangle_indices) {

	int t_num = triangle_indices.cols();
	int he_size = t_num * 6;
	std::vector<std::vector<int>> vv_incidents(he_size);
	std::vector<std::vector<int>> vv_he_pos(he_size);
	he_list_NEW.resize(he_size);
	triangle_list_NEW.resize(t_num);
	vertex_list_NEW.resize(he_size);


	int he_cur_size=0;
	int max_v_index = -1;
	for (int i = 0; i < t_num; i++) {
		v3i t = triangle_indices.col(i);
		std::vector<HE_NEW*> t_he(3);
		std::vector<int> t_he_pos(3);
		for (int j = 0; j < 3; j++) {
			int j_next = (j + 1) % 3;
			int v[] = { t[j] ,t[j_next]};
			//new he
			//HE_NEW& he = t_he[j];
			HE_NEW he ;
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

				HE_NEW oppo_he;
				oppo_he.v = v[0];
				int oppo_he_pos = he_pos + 1;

				he_list_NEW[oppo_he_pos] = oppo_he;
				vv_he_pos[v[1]].push_back(oppo_he_pos);
				vv_incidents[v[1]].push_back(v[0]);

				he_cur_size += 2;
			}
			he_list_NEW[he_pos] = he;
			t_he[j] = &he_list_NEW[he_pos];
			vv_he_pos[v[0]].push_back(he_pos);
			vv_incidents[v[0]].push_back(v[1]);


			if (vertex_list_NEW[v[0]].h == -1) {
				vertex_list_NEW[v[0]].h = he_pos;
				vertex_list_NEW[v[0]].i = v[0];
				max_v_index = std::max(max_v_index, v[0]);
			}
		}
		for (int j = 0; j < 3; j++) {
			int j_next = (j + 1) % 3;
			t_he[j]->n = t_he_pos[j_next];
		}

		triangle_list_NEW[i].h = t_he_pos[0];
		triangle_list_NEW[i].i = i;
	}
	he_list_NEW.resize(he_cur_size);
	vertex_list_NEW.resize(max_v_index+1);


}



std::function<std::vector<int>(const int )>  Topology_Computer::get_one_ring() {
	auto oppo_h = [](int h){
		int e = h / 2*2;
		int ehi = (h + 1) % 2;
		return e+ehi;
	};
	for (int i = 0; i < vertex_list_NEW.size(); i++) {

		int h = vertex_list_NEW[i].h;
		while (true) {

			if (he_list_NEW[h].t == -1) {
				vertex_list_NEW[i].h = h;
				break;
			}
			h = he_list_NEW[h].n;
			h = he_list_NEW[h].n;
			h = oppo_h(h);
		}
	}
	
	std::function<std::vector<int>(const int )> func = [&](const int i) {
		std::vector<int> ret;
		int h0 = vertex_list_NEW[i].h;
		int h = h0;
		while (true) {
			int v = he_list_NEW[h].v;
			ret.push_back(he_list_NEW[h].v);
			h = oppo_h(h);
			h = he_list_NEW[h].n;

			if (h == h0||h == -1) {
				break;

			}
		}
		return ret;
	};

	return  func;

}

void Topology_Computer::get_edge_indices(m2xi& edge_indices) {

	int e_num=he_list_NEW.size()/2;
	edge_indices.resize(2,e_num);
	for (int i = 0; i < e_num; i++) {
		edge_indices(0,i) = he_list_NEW[i*2+0].v;
		edge_indices(1,i) = he_list_NEW[i*2+1].v;
	}

}

