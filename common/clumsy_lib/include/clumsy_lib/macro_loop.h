
#pragma once

#define EVAL(...) EVAL256(__VA_ARGS__)
#define EVAL256(...) EVAL128(EVAL128(__VA_ARGS__))
#define EVAL128(...) EVAL64(EVAL64(__VA_ARGS__))
#define EVAL64(...) EVAL32(EVAL32(__VA_ARGS__))
#define EVAL32(...) EVAL16(EVAL16(__VA_ARGS__))
#define EVAL16(...) EVAL8(EVAL8(__VA_ARGS__))
#define EVAL8(...) EVAL4(EVAL4(__VA_ARGS__))
#define EVAL4(...) EVAL2(EVAL2(__VA_ARGS__))
#define EVAL2(...) EVAL1(EVAL1(__VA_ARGS__))
#define EVAL1(...) __VA_ARGS__

#define LOOP_0(PRE_MACRO,MACRO,...) 
#define LOOP_1(PRE_MACRO,MACRO,...) LOOP_0(PRE_MACRO,MACRO,__VA_ARGS__) PRE_MACRO(0,__VA_ARGS__)
#define LOOP_2(PRE_MACRO,MACRO,...) LOOP_1(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(1,__VA_ARGS__)
#define LOOP_3(PRE_MACRO,MACRO,...) LOOP_2(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(2,__VA_ARGS__)
#define LOOP_4(PRE_MACRO,MACRO,...) LOOP_3(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(3,__VA_ARGS__)
#define LOOP_5(PRE_MACRO,MACRO,...) LOOP_4(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(4,__VA_ARGS__)
#define LOOP_6(PRE_MACRO,MACRO,...) LOOP_5(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(5,__VA_ARGS__)
#define LOOP_7(PRE_MACRO,MACRO,...) LOOP_6(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(6,__VA_ARGS__)
#define LOOP_8(PRE_MACRO,MACRO,...) LOOP_7(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(7,__VA_ARGS__)
#define LOOP_9(PRE_MACRO,MACRO,...) LOOP_8(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(8,__VA_ARGS__)
#define LOOP_10(PRE_MACRO,MACRO,...) LOOP_9(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(9,__VA_ARGS__)
#define LOOP_11(PRE_MACRO,MACRO,...) LOOP_10(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(10,__VA_ARGS__)
#define LOOP_12(PRE_MACRO,MACRO,...) LOOP_11(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(11,__VA_ARGS__)
#define LOOP_13(PRE_MACRO,MACRO,...) LOOP_12(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(12,__VA_ARGS__)
#define LOOP_14(PRE_MACRO,MACRO,...) LOOP_13(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(13,__VA_ARGS__)
#define LOOP_15(PRE_MACRO,MACRO,...) LOOP_14(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(14,__VA_ARGS__)
#define LOOP_16(PRE_MACRO,MACRO,...) LOOP_15(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(15,__VA_ARGS__)
#define LOOP_17(PRE_MACRO,MACRO,...) LOOP_16(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(16,__VA_ARGS__)
#define LOOP_18(PRE_MACRO,MACRO,...) LOOP_17(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(17,__VA_ARGS__)
#define LOOP_19(PRE_MACRO,MACRO,...) LOOP_18(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(18,__VA_ARGS__)
#define LOOP_20(PRE_MACRO,MACRO,...) LOOP_19(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(19,__VA_ARGS__)
#define LOOP_21(PRE_MACRO,MACRO,...) LOOP_20(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(20,__VA_ARGS__)
#define LOOP_22(PRE_MACRO,MACRO,...) LOOP_21(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(21,__VA_ARGS__)
#define LOOP_23(PRE_MACRO,MACRO,...) LOOP_22(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(22,__VA_ARGS__)
#define LOOP_24(PRE_MACRO,MACRO,...) LOOP_23(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(23,__VA_ARGS__)
#define LOOP_25(PRE_MACRO,MACRO,...) LOOP_24(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(24,__VA_ARGS__)
#define LOOP_26(PRE_MACRO,MACRO,...) LOOP_25(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(25,__VA_ARGS__)
#define LOOP_27(PRE_MACRO,MACRO,...) LOOP_26(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(26,__VA_ARGS__)
#define LOOP_28(PRE_MACRO,MACRO,...) LOOP_27(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(27,__VA_ARGS__)
#define LOOP_29(PRE_MACRO,MACRO,...) LOOP_28(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(28,__VA_ARGS__)
#define LOOP_30(PRE_MACRO,MACRO,...) LOOP_29(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(29,__VA_ARGS__)
#define LOOP_31(PRE_MACRO,MACRO,...) LOOP_30(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(30,__VA_ARGS__)
#define LOOP_32(PRE_MACRO,MACRO,...) LOOP_31(PRE_MACRO,MACRO,__VA_ARGS__) MACRO(31,__VA_ARGS__)


#define LOOP(LOOP_COUNT,PRE_MACRO,MACRO,...) LOOP_##LOOP_COUNT(PRE_MACRO,MACRO,__VA_ARGS__)

#define EMP()
#define DEFF(m) m EMP()
#define _LOOP() LOOP
#define INSIDE_LOOP(LOOP_COUNT,PRE_MACRO,MACRO,...) DEFF(_LOOP)()(LOOP_COUNT,PRE_MACRO,MACRO,__VA_ARGS__)

#define PUSH_BACK_LIST(i,v) v.push_back(i);




