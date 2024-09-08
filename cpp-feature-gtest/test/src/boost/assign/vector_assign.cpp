#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
//#include <boost/numeric/ublas/vector.hpp>
//#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/assign/std/vector.hpp>

using namespace std;
using namespace boost::assign;
//using namespace boost::numeric::ublas; 
using namespace testing;  //mock 

TEST(vector_assign, append) {

    std::vector<int> a; a +=  1,2;
    std::vector<int> b; b +=  2,3;

    ASSERT_THAT(a, ElementsAre(1,2));
    ASSERT_THAT(b, ElementsAre(2,3));



}

