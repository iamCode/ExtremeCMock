#include <stdlib.h>
#include <stdio.h>
#define UNIT_TEST
#include "ExtremeCMock.h"
#include <ExtremeCUnit.h>
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

TEST(mock_one_func_cpp) {
	Assert(func1(1,2) == 10);
	mock_func((void*)func1,(void*)func2);
	Assert(func1(1,2) == 20);
	unmock_func((void*)func1);
	return 0;
}
TEST(mock_unmock_func_cpp) {
	Assert(func1(1,2) == 10);
	mock_func((void*)func1,(void*)func2);
	Assert(func1(1,2) == 20);
	unmock_func((void*)func1);
	Assert(func1(1,2) == 10);
	return 0;
}
TEST(mock_remock_func_cpp) {
	Assert(func1(1,2) == 10);
	mock_func((void*)func1,(void*)func2);
	Assert(func1(1,2) == 20);
	mock_func((void*)func1,(void*)func3);
	Assert(func1(1,2) == 30);
	unmock_func((void*)func1);
	Assert(func1(1,2) == 10);
	return 0;
}
TEST(mock_mock_call_from_func_cpp) {
	Assert(func1(1,2) == 10);
	Assert(func_call3(1,2) == 41);
	mock_func((void*)func1,(void*)func3);
	Assert(func1(1,2) == 30);
	Assert(func_call3(1,2) == 61);
	unmock_func((void*)func1);
	Assert(func_call3(1,2) == 41);
	return 0;
}

TEST(mock_mock_call_that_is_mocked_cpp) {
	Assert(func_call5(1,2) == 106);
	mock_func((void*)func1,(void*)func3);
	Assert(func_call5(1,2) == 126);
	mock_func((void*)func_call4,(void*)func_call3);
	Assert(func_call5(1,2) == 94);
	mock_func((void*)func1,(void*)func2);
	Assert(func_call5(1,2) == 84);
	unmock_all();
	Assert(func_call5(1,2) == 106);
	return 0;
}

#include <unistd.h>

int my_read(int fid, char * buffer, int size) {
	return 90;
}

//extern int __strcmp_sse2(const char* a, const char* b) ;
//extern int __strcmp_sse42(const char* a, const char* b) ;
//extern int __strcmp_sse3(const char* a, const char* b) ;
TEST(mock_libc_cpp) {
	char buf[10];
	MOCK_FUNC(read,my_read);
	AssertEqInt(read(-1,buf, 9), 90);
	unmock_all();// make sure you do this, read maybe I should make the unit test frame work call this 
	return 0;
}

class Foo_class {
	public:
	int method() {
		return 20;
	}
};

int method_bb(Foo_class * objp) {
	return 30;
}

TEST(mock_class_member) {
	Foo_class a;
	AssertEqInt(a.method(), 20);
	MOCK_FUNC(&Foo_class::method,method_bb);
	AssertEqInt(a.method(), 30);
	unmock_all();// make sure you do this, read maybe I should make the unit test frame work call this 
	return 0;
}
