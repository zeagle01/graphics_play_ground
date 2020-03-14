
#include "Simulator.h"
#include "PD_Simulator.h"
#include "FEM_Simulator.h"
#include "glog/logging.h"




std::map<std::string, Simulator::Simulator_Constructor> Simulator::m_constructors=std::map<std::string, Simulator::Simulator_Constructor>();

void Simulator::fill_types()
{
    m_constructors["PD"]=[&]()
    {
        auto sub= std::make_shared<PD_Simulator>();
        return std::dynamic_pointer_cast<Simulator>(sub);
    };

    m_constructors["FEM"]=[&]()
    {
        auto sub= std::make_shared<FEM_Simulator>();
        return std::dynamic_pointer_cast<Simulator>(sub);
    };

}
std::shared_ptr<Simulator> Simulator::new_instance(std::string type)
{
    if(m_constructors.empty())
    {
        fill_types();
    }

    if(m_constructors.count(type))
    {
        return m_constructors[type]();
    }
    else
    {
        LOG(ERROR) << "there is not such simulator:" + type;
    }
    return nullptr;
}
