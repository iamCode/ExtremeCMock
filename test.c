#include <stdlib.h>
#include <stdio.h>
#include "mock.h"
#include "unittest.h"
int func2(int arg1, int arg2) {
	return 20;
}
int  func1(int arg1,int arg2) {
	return 10;
}
int  func3(int arg1,int arg2) {
	return 30;
}


int  func_call3(int arg1,int arg2) {
	return 31 + func1(1,2);
}
int  func_call4(int arg1,int arg2) {
	return 32 + func_call3(1,2);
}
int  func_call5(int arg1,int arg2) {
	return 33 + func_call4(1,2);
}

TEST(mock_one_func) {
	Assert(func1(1,2) == 10);
	mock_func(func1,func2);
	Assert(func1(1,2) == 20);
	return 0;
}
TEST(mock_unmock_func) {
	Assert(func1(1,2) == 10);
	mock_func(func1,func2);
	Assert(func1(1,2) == 20);
	unmock_func(func1);
	Assert(func1(1,2) == 10);
	return 0;
}
TEST(mock_remock_func) {
	Assert(func1(1,2) == 10);
	mock_func(func1,func2);
	Assert(func1(1,2) == 20);
	mock_func(func1,func3);
	Assert(func1(1,2) == 30);
	unmock_func(func1);
	Assert(func1(1,2) == 10);
	return 0;
}
TEST(mock_mock_call_from_func) {
	Assert(func1(1,2) == 10);
	Assert(func_call3(1,2) == 41);
	mock_func(func1,func3);
	Assert(func1(1,2) == 30);
	Assert(func_call3(1,2) == 61);
	unmock_func(func1);
	Assert(func_call3(1,2) == 41);
	return 0;
}

TEST(mock_mock_call_that_is_mocked) {
	Assert(func_call5(1,2) == 106);
	mock_func(func1,func3);
	Assert(func_call5(1,2) == 126);
	mock_func(func_call4,func_call3);
	Assert(func_call5(1,2) == 94);
	mock_func(func1,func2);
	Assert(func_call5(1,2) == 84);
	unmock_all();
	Assert(func_call5(1,2) == 106);
	return 0;
}
