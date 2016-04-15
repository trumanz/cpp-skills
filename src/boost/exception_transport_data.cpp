#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <boost/exception/all.hpp>
#include <list>



#if 0
static void f() {
 // throw boost::unknown_exception();
    BOOST_THROW_EXCEPTION(boost::unknown_exception());
}
#endif


struct XExc : std::exception
{
   public:
      std::string x;
      std::list<int>  y;
      virtual ~XExc() _GLIBCXX_USE_NOEXCEPT {   };

};


struct Info{
public:
    Info(int code,  std::string msg){
         this->code = code;
         this->msg = msg;
    }
    int code;
    std::string msg;
};
#if 0
typedef boost::error_info<struct  Info, Info>  MyInfo;
struct my_error: virtual boost::exception, virtual std::exception { };
#endif

template<typename T>
struct MyException : virtual boost::exception, virtual std::exception
{
   MyException(const T& e){
       (*this) << MyInfo(e);
   }
   typedef boost::error_info<T, T>  MyInfo;
   const T* get(){
       return   boost::get_error_info<MyInfo>(*this);
   }
};

TEST(boost_exception, diagnostic) {
   
    try {  
      XExc x = XExc();
      x.x = "tesst";
      throw x;
    } catch (XExc &e){
       printf("%s\n", e.x.c_str());
    }

    int code = 0;
    std::string msg = "";
    try {
        throw MyException<Info>(Info(2, "forTest"));
    } catch (MyException<Info> &e) {
        code = e.get()->code;
        msg =  e.get()->msg;
    }
    ASSERT_EQ(2, code);
    ASSERT_EQ("forTest", msg);

    code = 0;
    try{ 
       throw MyException<int>(5);
    } catch (MyException<int> &e) {
        code = *e.get();
        printf("%s\n", e.what());
    }
    ASSERT_EQ(5, code);

}
#if 0
TEST(boost_exception, diagnostic) {
    try {
         f();
    } catch (boost::exception & e ) {
          printf("%s\n",  diagnostic_information(e).c_str());
    }

    try {
        throw  my_error() <<  MyInfo(Info(12, "test"));
    } catch ( my_error &e){
        const Info* info = boost::get_error_info<MyInfo>(e);
        printf("%s\n", info->msg.c_str());
    }
   
    throw MyException<Info>(Info(2, "sss")); 

}
#endif
