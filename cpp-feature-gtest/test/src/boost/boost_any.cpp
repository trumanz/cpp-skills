#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include "boost/any.hpp"
using namespace std;

class X {
public:
    int a;
};
class Y {
public:
    int a;
};

TEST(boost_any_test, baisc_type) {

    boost::any int_x =  (int)1;
    ASSERT_TRUE( int_x.type() == typeid(int));
    ASSERT_TRUE( int_x.type() != typeid(unsigned int));

    boost::any X_x =  X();
    ASSERT_TRUE( X_x.type() == typeid(X));
    ASSERT_TRUE( X_x.type() != typeid(Y));

}

