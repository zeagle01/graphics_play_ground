
#ifndef PLOT_DATA_IO_H
#define PLOT_DATA_IO_H

#include <string>
#include <memory>
#include <fstream>
#include <map>
#include <functional>

class Plot_Data_IO{
    public:

        static std::shared_ptr<Plot_Data_IO> get_instance();

        enum class  Print_Mode{OR,AP};
        enum class  Polygon_Type{edge=2,triangle=3,tet=4,box=8};

        template <typename T>
        void write_few_numbers(const std ::string &file,
                               const std::initializer_list<T> &nums,
                               const Print_Mode pm = Print_Mode::OR);

        template <typename T>
        void write_flattened_tensor(const std ::string &file,
                                    const T *data,
                                    const std::initializer_list<int> &dim,
                                    const Print_Mode pm = Print_Mode::OR);

        template <typename T>
        void write_key_file(const std ::string &file,
                            const Polygon_Type polygon_type,
                            const std::vector<T> &X,
                            const std::vector<int32_t>& EV,
                             const Print_Mode pm=Print_Mode::OR);

        template <typename T>
        void write_key_file(const std::string &file,
                            const Polygon_Type &polygon_type,
                            const int32_t &v_num,
                            const int32_t &e_num,
                            const T *X,
                            const T *VX,
                            const int32_t *EV,
                            const T *VE,
                            const Print_Mode pm=Print_Mode::OR);

        // tecplot triangle mesh
        template <typename T>
        void write_tecplot_triangle_mesh(const std::string &file,
                                         const std::vector<T> &X,
                                         const std::vector<int32_t> &indices,
                                         const Print_Mode pm = Print_Mode::OR);
        template <typename T>
        void write_tecplot_FEM_mesh(const std::string &file,
                          const size_t &vNum,
                          const std::function<void(int32_t, std::vector<T> &)> getVariableValue,
                          const std::vector<int32_t> &indices,
                          const Print_Mode pm = Print_Mode::OR);

        template <typename T>
        void read_tensor(const std::string &file,
                         std::vector<T> &tensor,
                         std::vector<int32_t> &dim);

        //plot helpers
    private:
        void write_tensor_shape(std::ofstream &fout, const std::vector<int> &dim_v);

        //memory management
    private:
        static std::shared_ptr<Plot_Data_IO> singleton;

        struct Ofstream_Asocciate
        {
            std::shared_ptr<std::ofstream> fout = nullptr;

            // base
            int num_accesed = 0;

            //key files
            int eNum = 0;
            int groupNum = 0;
            //.plt mesh file

            //.plt animation file
            int strand_id=0;
            //float solution_time=0;

            std::ofstream &get_fout()
            {
                return *(fout.get());
            }
    };

    std::shared_ptr<Ofstream_Asocciate> get_or_create_ofstream(const std::string& file_name,Print_Mode pm);
    std::map<std::string ,std::shared_ptr<Ofstream_Asocciate>> ofstreams;
};

#include "plot_data_IO.cpp"

#endif