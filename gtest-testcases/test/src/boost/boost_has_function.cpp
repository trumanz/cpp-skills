#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <iostream>
#include <typeinfo>
#include <iomanip>
#include <sstream>
#include <boost/tti/has_member_function.hpp>
#include <boost/mpl/if.hpp>
#include <string>
struct TypeHasMemberFunction{
   std::string toString() { return "TypeHasMemberFunction"; };
};

struct TypeNonMemberFunction {
    TypeNonMemberFunction(){
           x = "TypeNonMemberFunction";
    }
    std::string x;
};

std::string toString(TypeNonMemberFunction x) {
   return x.x;
}

BOOST_TTI_HAS_MEMBER_FUNCTION(toString)


class MemberFunctonCaller{
public:
   template<typename T>
   std::string call(T x){
        return x.toString();
   }
};
class OutFunctionCaller{
public:
   template<typename T>
   std::string call(T x){
        return toString(x);
   }
};


template<typename T>
std::string getString(T x)
{
    typedef typename boost::mpl::if_c<has_member_function_toString<T, std::string>::value, MemberFunctonCaller, OutFunctionCaller>::type CallerT;
    //std::cout <<typeid(x).name()<< "\n";
    //std::cout << has_member_function_toString<T, int>::value   << "\n";
    //std::cout <<typeid(t1).name()<< "\n";
    //std::cout << t1().call(x) <<"\n";
    return  CallerT().call(x);
}

TEST(boost_has_function, choose_call) {
     std::string x1 = getString(TypeHasMemberFunction());
     std::string x2 = getString(TypeNonMemberFunction());

    ASSERT_TRUE( x1 == "TypeHasMemberFunction");
    ASSERT_TRUE( x2 == "TypeNonMemberFunction");

}

