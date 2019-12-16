
import numpy as np
import copy



def assertArrayEquals(testCase,act_arr,exp_arr):
    testCase.assertTrue(np.array_equal(act_arr,exp_arr))

def assertArrayEqualsUnordedly(testCase,act_arr,exp_arr):
    unorder_act_arr=np.sort(act_arr)
    unorder_exp_arr=np.sort(exp_arr)
    testCase.assertTrue(np.array_equal(unorder_act_arr,unorder_exp_arr))
