#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "gtest/gtest.h"
#include "boost/date_time/posix_time/posix_time.hpp" 


class StrTime {
private:
   std::locale locale_input;
   std::locale locale_output;
public:
    StrTime(const char *pattern = NULL) {
         if(pattern == NULL) {
             //"2005-10-15 13:14:15.003400"
             pattern = "%Y-%m-%d %H:%M:%S.%f";
         }
         locale_input = std::locale(std::locale::classic(),new boost::posix_time::time_input_facet(pattern));
         locale_output = std::locale(std::locale::classic(),new boost::posix_time::time_facet(pattern));
    }
    int parser(const std::string& timeStr, boost::posix_time::ptime *ptOut){
         boost::posix_time::ptime pt;
         std::istringstream is(timeStr);
         is.imbue(locale_input);
         //printf("%s\n", is.str().c_str());
         is >> pt;
         if(pt == boost::posix_time::ptime()) {
              return -1;
         }
         *ptOut = pt;
         return 0;
    }
    std::string format(const boost::posix_time::ptime &pt){
         std::ostringstream os;
         os.imbue(locale_output);
         os << pt;
         return os.str();
    }
};


TEST(boost_datetime, posix_time_parse_format) {

   using namespace boost::posix_time;
   //ptime pt_now = second_clock::local_time();
   ptime pt;
   std::string str;
   int ret;
    
   StrTime tf("%Y-%m-%d %H:%M:%S");

   //wrong month 13
   ret = tf.parser("2004-13-21 12:45:33", &pt);
   EXPECT_EQ(-1, ret);

   //time 25hour will increase day
   ret = tf.parser("2004-03-21 25:45:33", &pt);
   EXPECT_EQ(0, ret);
   str =  tf.format(pt);
   EXPECT_EQ("2004-03-22 01:45:33", str);

   //miss seconds, will be zero
   ret = tf.parser("2004-03-21 25:45", &pt);
   str =  tf.format(pt);
   EXPECT_EQ(0, ret);
   EXPECT_EQ("2004-03-22 01:45:00", str);

   //wrorng format
   ret = tf.parser("2004--03-21 12:45:33", &pt);
   EXPECT_EQ(-1, ret);

   ret = tf.parser("2004-03-21 12:45:33", &pt);
   EXPECT_EQ(0, ret);

   str =  tf.format(pt);
   EXPECT_EQ("2004-03-21 12:45:33", str);

}

TEST(boost_datetime, posix_time_duration) {

   using namespace boost::posix_time;
   StrTime tf("%Y-%m-%d %H:%M:%S");
   int ret;
   ptime pt1;
   ptime pt2;
   time_duration td;

   ret = tf.parser("2000-01-01 00:00:00", &pt1);
   EXPECT_EQ(0, ret);

   ret = tf.parser("2000-01-02 20:00:00", &pt2);
   EXPECT_EQ(0, ret);

   td = pt2 - pt1;
 
   std::cout <<  td << "\n";

}

