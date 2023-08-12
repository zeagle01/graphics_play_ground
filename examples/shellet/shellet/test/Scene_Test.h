
#ifndef SCENE_TEST_H
#define SCENE_TEST_H

#include "gtest/gtest.h"
#include "Scene.h"

class Scene_Test:public ::testing::Test{

    protected:
	virtual void SetUp();
	Scene* scene;

};

#endif
