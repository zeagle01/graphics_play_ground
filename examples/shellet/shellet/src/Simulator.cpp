
#include "Simulator.h"
#include "PD_Simulator.h"
#include "FEM_Simulator.h"
#include "glog/logging.h"



std::shared_ptr<Simulator> Simulator::new_instance(std::string type)
{

	using Simulator_Constructor = std::function<std::shared_ptr<Simulator>()>;
    static std::map<std::string, Simulator_Constructor> constructors{};
    if (constructors.empty())
    {
        constructors["PD"] = [&]() {
            auto sub = std::make_shared<PD_Simulator>();
            return std::dynamic_pointer_cast<Simulator>(sub);
        };

        constructors["FEM"] = [&]() {
            auto sub = std::make_shared<FEM_Simulator>();
            return std::dynamic_pointer_cast<Simulator>(sub);
        };
    }
    if(constructors.count(type))
    {
        return constructors[type]();
    }
    else
    {
        LOG(ERROR) << "there is not such simulator:" + type;
    }
    return nullptr;
}
