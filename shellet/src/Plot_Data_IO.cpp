
#include "Plot_Data_IO.h"
#include <fstream>

std::shared_ptr<Plot_Data_IO> Plot_Data_IO::singleton =nullptr;
std::shared_ptr<Plot_Data_IO> Plot_Data_IO::get_instance()
{
    if(!singleton){
        singleton = std::make_shared<Plot_Data_IO>();
    }
    return singleton;
}

std::shared_ptr<Plot_Data_IO::Ofstream_Asocciate> Plot_Data_IO::get_or_create_ofstream(const std::string& file_name,Print_Mode pm)
{
    if(!ofstreams.count(file_name)||pm==Print_Mode::OR){
        ofstreams[file_name] = std::make_shared<Ofstream_Asocciate>();
        ofstreams[file_name]->fout = std::make_shared<std::ofstream>(file_name);
    }
    return ofstreams[file_name];
}

template <typename T>
void Plot_Data_IO::write_few_numbers(const std ::string &file, const std::initializer_list<T> &nums, Print_Mode pm )
{

    std::vector<T> nums_v(nums);

    write_flattened_tensor(file, nums_v.data(), { int(nums_v.size()) }, pm);
}


void
Plot_Data_IO::write_tensor_shape(std::ofstream &fout, const std::vector<int>& dim_v)
{
    fout << "# ";
    for (int i = 0; i < dim_v.size(); i++) {
        fout << dim_v[i] << " ";
    }
    fout << std::endl;
}

template <typename T>
void Plot_Data_IO::write_flattened_tensor(const std ::string &file, const T *data, const std::initializer_list<int> &dim, const Print_Mode pm)
{
    auto stream_asocciate   = get_or_create_ofstream(file, pm);
    std::ofstream &fout =stream_asocciate->get_fout();
    if (stream_asocciate->num_accesed==0) {
        std::vector<int> dim_v(dim);
        write_tensor_shape(fout, dim_v);
    }
    int size = 1;
    std::vector<int> dim_v(dim);
    for (int i = 0; i < dim_v.size(); i++) {
        size *= dim_v[i];
    }

    for (int i = 0; i < size; i++) {
        fout << data[i] << std::endl;
    }
}

std::vector<std::vector<int>> compute_index_tupe_through_shape(const std::initializer_list<int> &dim){
    std::vector<int> shape(dim);
    std::vector<std::vector<int>> index_tuple;
    index_tuple.push_back(std::vector<int>());
    for (int shape_i = 0; shape_i < shape.size(); shape_i++)
    {
        std::vector<std::vector<int>> index_tuple_new;
        for (int index_tuple_j = 0; index_tuple_j < index_tuple.size(); index_tuple_j++)
        {
            std::vector<std::vector<int>> index_tuple_j_cartisian_multiply_by_shape_i;
            for (int shape_i_k = 0; shape_i_k < shape[shape_i]; shape_i_k++)
            {
                std::vector<int> index_tuple_j_append_shape_i_k(index_tuple[index_tuple_j]);
                index_tuple_j_append_shape_i_k.push_back(shape_i_k);
                index_tuple_j_cartisian_multiply_by_shape_i.push_back(index_tuple_j_append_shape_i_k);
            }
            index_tuple_new.insert(index_tuple_new.end(), index_tuple_j_cartisian_multiply_by_shape_i.begin(), index_tuple_j_cartisian_multiply_by_shape_i.end());
        }
        index_tuple = index_tuple_new;
    }

    return index_tuple;
}


template <typename T>
void Plot_Data_IO::write_shaped_tensor(const std ::string &file,
                                       const std::function<T(const std::vector<int> &index_tuple)> getData,
                                       const std::initializer_list<int> &dim,
                                       const Print_Mode pm)
{
    auto stream_asocciate = get_or_create_ofstream(file, pm);
    std::ofstream &fout = stream_asocciate->get_fout();
    if (stream_asocciate->num_accesed == 0)
    {
        std::vector<int> shape(dim);
        write_tensor_shape(fout, shape);
    }
//    std::vector<int> shape(dim);
//    std::vector<std::vector<int>> index_tuple;
//    index_tuple.push_back(std::vector<int>());
//    for (int shape_i = 0; shape_i < shape.size(); shape_i++)
//    {
//        std::vector<std::vector<int>> index_tuple_new;
//        for (int index_tuple_j = 0; index_tuple_j < index_tuple.size(); index_tuple_j++)
//        {
//            std::vector<std::vector<int>> index_tuple_j_cartisian_multiply_by_shape_i;
//            for (int shape_i_k = 0; shape_i_k < shape[shape_i]; shape_i_k++)
//            {
//                std::vector<int> index_tuple_j_append_shape_i_k(index_tuple[index_tuple_j]);
//                index_tuple_j_append_shape_i_k.push_back(shape_i_k);
//                index_tuple_j_cartisian_multiply_by_shape_i.push_back(index_tuple_j_append_shape_i_k);
//            }
//            index_tuple_new.insert(index_tuple_new.end(), index_tuple_j_cartisian_multiply_by_shape_i.begin(), index_tuple_j_cartisian_multiply_by_shape_i.end());
//        }
//        index_tuple = index_tuple_new;
//    }
    auto index_tuple=compute_index_tupe_through_shape(dim);
    for (int i = 0; i < index_tuple.size(); i++)
    {
        fout << getData(index_tuple[i]) << std::endl;
    }
}

template <typename T>
void Plot_Data_IO::write_key_file(const std ::string &file,
                            const Polygon_Type polygon_type,
                            const std::vector<T> &X,
                            const std::vector<int32_t>& EV,
                             const Print_Mode pm)
{
    write_key_file(file,
                   polygon_type,
                   X.size() / 3,
                   EV.size() / (int32_t)polygon_type,
                   &X[0],
                   (T*)nullptr,
                   &EV[0],
                   (T*)nullptr,
                   pm);
}

template <typename T>
void Plot_Data_IO::write_key_file(const std::string &file,
                                  const Polygon_Type &polygon_type,
                                  const int32_t &v_num,
                                  const int32_t &e_num,
                                  const T *X,
                                  const T *VX,
                                  const int32_t *EV,
                                  const T *VE,
                                  const Print_Mode pm)
{

    if (v_num == 0) {
        return;
    }
    auto stream_asocciate   = get_or_create_ofstream(file, pm);
    std::ofstream &fout =stream_asocciate->get_fout();
    fout << std::setprecision(8);

    int32_t max_index_in_a_group   = 1000000;
    int32_t group_begin_index = stream_asocciate->groupNum * max_index_in_a_group;
    {
        if (VE) {
            std::stringstream ss;
            ss << "$SHELL_ELEMENT_RESULTS RESULT\n";
            ss << "$RESULT OF "
               << "element_group_" << group_begin_index << "\n";
            fout << ss.str();
            for (size_t i = 0; i < e_num; i++) {
                int32_t e_id = group_begin_index + i + 1;
                fout << e_id << "," << VE[i];
                fout << std::endl;
            }
        }
    }
    {
        //        for (size_t i = 0; i < nodeVarNum; i++) {
        //            std::stringstream ss;
        //            ss << "$SHELL_ELEMENT_RESULTS RESULT\n";
        //            ss << "$RESULT OF "
        //               << "node_group_" << groupBegin << "var_" << i << "\n";
        //            fout << ss.str();
        //            for (size_t j = 0; j < vNum; j++) {
        //                int32_t v_id = groupBegin + j + 1;
        //                fout << v_id << "," << VX[i][j];
        //                fout << std::endl;
        //            }
        //        }
        if (VX) {
            std::stringstream ss;
            ss << "$SHELL_NODE RESULT\n";
            ss << "$RESULT OF "
               << "node_group_" << group_begin_index << "\n";
            fout << ss.str();
            for (size_t i = 0; i < v_num; i++) {
                int32_t v_id = group_begin_index + i + 1;
                fout << v_id << "," << VX[i];
                fout << std::endl;
            }
        }
    }
    {
        fout << "*NODE" << std::endl;
        for (size_t i = 0; i < v_num; i++) {
            int32_t v_id = group_begin_index + i + 1;
            fout << v_id << ",";
            for (int j = 0; j < 3; j++) {
                fout << X[i * 3 + j] << ",";
            }
            fout << std::endl;
        }
    }
    {
        std::map<int, std::string> element_type_string_in_key_file;
        element_type_string_in_key_file[2] = "*ELEMENT_BEAM";
        element_type_string_in_key_file[3] = "*ELEMENT_SHELL";
        element_type_string_in_key_file[4] = "*ELEMENT_SOLID";
        element_type_string_in_key_file[8] = "*ELEMENT_SOLID";
        fout << element_type_string_in_key_file[(int32_t)polygon_type] << std::endl;
        for (size_t i = 0; i < e_num; i++) {
            int32_t e_id = group_begin_index + i + 1;
            fout << e_id << "," << group_begin_index + 1 << ",";
            for (size_t j = 0; j < (int32_t)polygon_type; j++) {
                int v_id = group_begin_index + EV[i * (int32_t)polygon_type + j] + 1;
                fout << v_id << ",";
            }
            fout << std::endl;
        }
    }

    stream_asocciate->groupNum++;
}

template <typename T>
void Plot_Data_IO::write_tecplot_triangle_mesh(const std::string &file,
                                          const std::vector<T> &X,
                                          const std::vector<int32_t> &indices,
                                          const Print_Mode pm)
{

    if (X.empty()) {
        return;
    }
    size_t varNum = 3;
    size_t vNum=X.size() / 3;
    std::function<void(int32_t vi, std::vector<T> & variables)> get_variables = [&](int32_t vi, std::vector<T> &variables) {
        variables.resize(varNum);
        variables[0] = X[vi * 3 + 0];
        variables[1] = X[vi * 3 + 1];
        variables[2] = X[vi * 3 + 2];
    };
    write_tecplot_FEM_mesh(file, vNum,
                 get_variables,
                 indices, pm);
}

template <typename T>
void Plot_Data_IO::write_tecplot_structure_data(const std ::string &file,
                                                const T *data,
                                                const std::initializer_list<int> &dim){
    // std::ofstream fout(file);

    auto stream_asocciate = get_or_create_ofstream(file, Plot_Data_IO::Print_Mode::OR);
    std::ofstream &fout = stream_asocciate->get_fout();
    std::vector<int> dim_v(dim);
    if (dim_v.size() > 4 ||dim_v.size()<2)
    {
        return;
    }
    std::vector<std::string> coods_variables{"x", "y", "z"};
    std::vector<std::string> ijk{"i", "j", "k"};
    std::string variable_line = "variables=";
    std::string zone_line = "zone ";

    int coods_num             = dim_v.size() - 1;
    int variable_num          = dim_v.back();
    int col_num               = coods_num +variable_num;
    int grid_point_size       = 1;
    for (int i = 0; i < coods_num; i++)
    {
        variable_line += "\"" + coods_variables[i] + "\",";
        std::stringstream ss;
        ss << ijk[i] << "=" << dim_v[i] << ", ";
        zone_line += ss.str();
        grid_point_size *= dim_v[i];
    }
    for (int i = 0; i < variable_num; i++)
    {
        std::stringstream ss;
        ss << "\""
           << "v" << i << "\", ";
        variable_line += ss.str();
    }
    zone_line += "f=point";

    fout << variable_line << std::endl;
    fout << zone_line << std::endl;
    for (int i = 0; i < grid_point_size; i++)
    {
        for (int j = 0; j < coods_num; j++)
        {
            fout << data[i * col_num + j] << " ";
        }
        for (int j = 0; j < variable_num; j++)
        {
            fout << data[i * col_num + coods_num + j] << " ";
        }
        fout << std::endl;
    }
}

template <typename T>
void Plot_Data_IO::write_tecplot_FEM_mesh(const std::string &file,
                                          const size_t &vNum,
                                          const std::function<void(int32_t, std::vector<T> &)> getVariableValue,
                                          const std::vector<int32_t> &indices,
                                          const Print_Mode pm ){
    std::vector<T> rec;
    getVariableValue(0, rec);
    size_t varNum = rec.size();

    auto stream_asocciate   = get_or_create_ofstream(file, pm);
    std::ofstream &fout =stream_asocciate->get_fout();

    std::stringstream header;

    if (varNum == 0) {
        return;
    }

    header << "zone n=" << vNum << ",e=" << indices.size() / 3
           << ",f=fepoint,et=triangle,strandid=" << stream_asocciate->strand_id
           << ",solutiontime=" << stream_asocciate->strand_id << "\n";

    fout << header.str();
    for (size_t vi = 0; vi < vNum; vi++) {
        std::vector<T> vars;
        getVariableValue(vi, vars);
        for (size_t vari = 0; vari < varNum; vari++) {
            fout << vars[vari] << " ";
        }
        fout << std::endl;
    }

    for (int32_t i = 0; i < indices.size() / 3; i++) {
        for (int32_t j = 0; j < 3; j++) {
            fout << indices[i * 3 + j] + 1 << " ";
        }
        fout << std::endl;
    }



                                          }

template <typename T>
void Plot_Data_IO::read_tensor(const std::string &file, std::vector<T> &tensor, std::vector<int32_t> &dim)
{
    tensor.clear();
    std::ifstream fin(file);
    std::string line;
    getline(fin, line);
    std::stringstream ss;
    ss << line;
    std::string word;
    getline(ss, word, ' ');
    int size = 1;
    while (getline(ss, word, ' '))
    {
        int d;
        std::stringstream word_ss;
        word_ss << word;
        word_ss >> d;
        dim.push_back(d);
        size *= d;
    }

    while (getline(fin, line))
    {
        T v;
        std::stringstream line_ss;
        line_ss << line;
        line_ss >> v;
        tensor.push_back(v);
    }
    int actSize = tensor.size();
    int frameNum = actSize / size;
    if (frameNum > 1)
    {
        dim.insert(dim.begin(), frameNum);
    }
}
