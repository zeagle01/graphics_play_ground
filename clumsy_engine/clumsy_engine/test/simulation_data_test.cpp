
#include "gmock/gmock.h" 

#include "simulation_data.h"


using namespace testing;
using namespace clumsy_engine;


//////////////////////////////////////////////////////////
template<typename sender_T>
struct Foward
{
	template<typename sim_acc_T, typename T>
	static void apply(sim_acc_T& datas, T& d)
	{
		auto input = datas.get<sender_T>();
		d = input;
	}
};

class Dependent_Data_Test:public Test
{
protected:

	void SetUp()override
	{
		m_data0 = std::make_shared<Node0>();
		m_data1 = std::make_shared<Node1>();

		m_data1->add_sender(m_data0);
	}

	class Node0 :public Dependent_Data<int> { };


	class Node1 :public Dependent_Data<float, Foward<Node0>, type_list<Node0>> {};


	std::shared_ptr<Node0 >m_data0;
	std::shared_ptr<Node1 >m_data1;

};



TEST_F(Dependent_Data_Test, two_nodes)
{
	m_data0->set(2);

	EXPECT_THAT(m_data1->get(), Eq(2.f));

	m_data0->set(3);

	EXPECT_THAT(m_data1->get(), Eq(3.f));

}


class Dependent_Chain :public Dependent_Data_Test
{
protected:

	void SetUp()override
	{

		m_data0 = std::make_shared<Node0>();
		m_data1 = std::make_shared<Node1>();
		m_data2 = std::make_shared<Node2>();

		m_data1->add_sender(m_data0);
		m_data2->add_sender(m_data1);
	}

	class Node2 :public Dependent_Data<double, Foward<Node1>,type_list<Node1>> {};


	std::shared_ptr<Node0> m_data0;
	std::shared_ptr<Node1> m_data1;
	std::shared_ptr<Node2> m_data2;

};

TEST_F(Dependent_Chain, three_nodes)
{
	m_data0->set(2);

	EXPECT_THAT(m_data1->get(), Eq(2.f));
	EXPECT_THAT(m_data2->get(), Eq(2.));

	m_data0->set(3);

	EXPECT_THAT(m_data1->get(), Eq(3.f));
	EXPECT_THAT(m_data2->get(), Eq(3.));

}
