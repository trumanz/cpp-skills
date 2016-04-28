#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "gtest/gtest.h"
#include "boost/date_time/posix_time/posix_time.hpp" 


class PosixTime {
private:
   std::locale locale;
public:
    PosixTime() {
         locale = std::locale(std::locale::classic(),new boost::posix_time::time_input_facet("%Y-%m-%d %H:%M:%S"));
    }
    int getPtime(const std::string& timeStr, boost::posix_time::ptime *ptOut){
         boost::posix_time::ptime pt;
         std::istringstream is(timeStr);
         is.imbue(locale);
         printf("%s\n", is.str().c_str());
         is >> pt;
         if(pt == boost::posix_time::ptime()) {
              return -1;
         }
         *ptOut = pt;
         return 0;
    }
};


TEST(boost_datetime, posix_time_parse_format) {

   using namespace boost::posix_time;
   ptime pt_now = second_clock::local_time();
   ptime pt;


   std::locale locale = std::locale(std::locale::classic(),new time_input_facet("%Y-%m-%d %H:%M:%S"));
  
   std::string s = "2004-03-21 12:45:33";
   std::istringstream is(s);
   is.imbue(locale);
   
   printf("%s\n", is.str().c_str());
   
   is >> pt;
   if(pt != ptime()) {
       printf("%s\n", to_iso_string(pt).c_str());
   } 

   

    
   //EXPECT_EQ(it, es.end());

}

