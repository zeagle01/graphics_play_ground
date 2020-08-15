#ifndef MESH_LOADER_TEST_H
#define MESH_LOADER_TEST_H

#include "Mesh_Loader.h"

#include "gtest/gtest.h"



class Mesh_Loader_Test:public ::testing::Test{
    protected:
	void SetUp();

	Mesh_Loader* mesh_loader;
};

#endif
