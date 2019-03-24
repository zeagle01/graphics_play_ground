
#include "Mesh_Loader.h"


#include "glog/logging.h"
#include <fstream>
#include <sstream>

void Mesh_Loader::load_from_obj(std::string obj_file){
    std::ifstream fin(obj_file);
    if(!fin.good()){
	std::stringstream ss;
	ss<<"can't load obj from "<<obj_file;
	//std::string error_massage=ss.str();
	LOG(ERROR)<<ss.str();
    }
    std::string line;
    while(getline(fin,line)){
	std::stringstream words;
	words<<line;
	std::string flag;
	words>>flag;
	if(flag=="v"){
	    for(int i=0;i<3;i++){
		float v;
		words>>v;
		m_positions.push_back(v);
	    }
	}else if(flag=="f"){
	    for(int i=0;i<3;i++){
		int v;
		words>>v;
		v-=1;
		m_indices.push_back(v);
	    }
	}
    }

}
std::vector<int> Mesh_Loader::get_indices(){
    return m_indices;
}
std::vector<float> Mesh_Loader::get_positions(){
    return m_positions;
}
bool Mesh_Loader::is_texture_loaded(){
    return false;
}
