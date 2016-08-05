#include <string>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <string>

using namespace std;

int diff(const string& date1, const string& date2){
   int rc;

   int year;
   int month;
   int day;

   struct tm tm1;
   struct tm tm2;

   memset(&tm1, 0, sizeof(tm1));
   memset(&tm2, 0, sizeof(tm2));

   rc = sscanf(date1.c_str(), "%04d%02d%02d", &year, &month, &day);
   assert(rc == 3);
   printf("date1:  %d, %d, %d\n", year, month, day); 
   tm1.tm_year = year-1900;
   tm1.tm_mon = month -1;
   tm1.tm_mday = day;
   
   rc = sscanf(date2.c_str(), "%04d%02d%02d", &year, &month, &day);
   assert(rc == 3);
   printf("date2:  %d, %d, %d\n", year, month, day); 
   tm2.tm_year = year-1900;
   tm2.tm_mon = month -1;
   tm2.tm_mday = day;

   time_t diff_seconds = mktime(&tm2) - mktime(&tm1);
   
   rc = diff_seconds/(60*60*24);
   printf("diff days between %s  and %s is %d\n", date1.c_str(), date2.c_str(), rc);
   return rc; 
 
}

TEST(mktime, diff_days) {
    int diff_days;
    diff_days = diff("20110101", "20110105");
    EXPECT_EQ(diff_days, 4);
    diff_days = diff("20110105", "20110101");
    EXPECT_EQ(diff_days, -4);
}
