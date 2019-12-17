
import numpy as np
import copy



def assertArrayEquals(testCase,act_arr,exp_arr):
    testCase.assertTrue(np.array_equal(act_arr,exp_arr))

def assertArrayEqualsUnordedly(testCase,act_arr,exp_arr):

#    data=np.array([[2,2],[0,1],[1,3],[0,2]])
#    index=np.lexsort(data.transpose())
#    #index=np.lexsort(data)
#    data=data[index]

    unorder_act_arr = np.array(act_arr)
    unorder_exp_arr = np.array(exp_arr)
    if len(unorder_exp_arr.shape)>1:
        unorder_act_arr=np.sort(unorder_act_arr)
        unorder_exp_arr=np.sort(unorder_exp_arr)
        unorder_act_arr_index=np.lexsort(unorder_act_arr.transpose())
        unorder_exp_arr_index=np.lexsort(unorder_exp_arr.transpose())
        unorder_act_arr=unorder_act_arr[unorder_act_arr_index]
        unorder_exp_arr=unorder_exp_arr[unorder_exp_arr_index]
        testCase.assertTrue(np.array_equal(unorder_act_arr,unorder_exp_arr))
    else:
        unorder_act_arr = np.sort(act_arr)
        unorder_exp_arr = np.sort(exp_arr)
        testCase.assertTrue(np.array_equal(unorder_act_arr, unorder_exp_arr))
