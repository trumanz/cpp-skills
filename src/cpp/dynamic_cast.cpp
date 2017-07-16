#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>


class A
{
public:
    virtual ~A(){}
};

class B
{
public:
    virtual ~B(){}
};

class D
{
public:
    virtual ~D(){}
};


class C : public A, public B
{
};


TEST(cpp_dynamic, multi_inherit_cast) {
    C* c = new C();

    B* b = dynamic_cast<B*>(c);
    EXPECT_TRUE(b != NULL);   

    A* a = dynamic_cast<A*>(b);
    EXPECT_TRUE(a != NULL);   

    D* d = dynamic_cast<D*>(b);
    EXPECT_TRUE(d == NULL);   
}
