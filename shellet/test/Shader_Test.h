
#ifndef SHADER_TEST_H
#define SHADER_TEST_H

#include "gtest/gtest.h"
#include "Shader.h"

class Shader_Test:public ::testing::Test{
    public:
	Shader_Test();

    protected:
	Shader* shader;
	void SetUp();
	

};


#endif
