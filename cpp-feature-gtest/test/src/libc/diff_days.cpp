#include <string>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <string>

using namespace std;

bool is_leap_year(int year){
     if( year%400 == 0) return true;
     if( year%4 == 0 && year%100 != 0) return true;
     return false;
}

//[year1, year2)
int count_leadp_years(int year1, int year2){
    assert(year2 > year1);
    int count = 0;
    for(int i = year1; i < year2; i++) {
            if(is_leap_year(i)) {
                count++;
            }
    }
    return count;
}

int count_days_of_year(int year, int month, int day){
   static const int days_each_month[] = {31,28/*leap year 29*/,31,30,31,30,31,31,30,31,30,31 };
   static int days_count_until_month[12];
   int days = 0;
   //thread unsafe
   static int inited = false;
   if(!inited){
       int sum = 0;
       for(int i = 0; i < 12; i++){
           days_count_until_month[i] = sum;
           sum += days_each_month[i];
       }
       inited = true;
   }
   bool is_leap = is_leap_year(year);
   assert(month >= 1);
   assert(day >= 1);

   days =  days_count_until_month[month-1] + day + (month > 2 && is_leap ? 1 : 0);
   printf("count_days_of_year, %d,%d,%d,  days=%d\n", year, month, day, days); 
   return days;
}

int diff_days(const string& date1, const string& date2){
   int year1;
   int month1;
   int day1;
   int year2;
   int month2;
   int day2;
   int rc;
   
   rc = sscanf(date1.c_str(), "%04d%02d%02d", &year1, &month1, &day1);
   assert(rc == 3);
   //printf("date1:  %d, %d, %d\n", year1, month1, day1); 

   rc = sscanf(date2.c_str(), "%04d%02d%02d", &year2, &month2, &day2);
   assert(rc == 3);
   //printf("date2:  %d, %d, %d\n", year2, month2, day2); 

   assert(year2 >=  year1);
   if(year2 == year1) {
        assert(month2 >= month1);
        if(month2 == month1) {
            assert(day2 >= day1);
        }
   }   

   int sum = 0;
    
   if(year2 > year1 + 1) {
      int leap_years = count_leadp_years(year1 + 1, year2);
      sum = 365*(year2-year1-1);
      sum += leap_years;
   }

   int days1_sum = count_days_of_year(year1, month1, day1);
   int days2_sum = count_days_of_year(year2, month2, day2);
   if(year2 == year1){
        sum += days2_sum - days1_sum;
   } else {
        sum += days2_sum;
        sum += is_leap_year(sum) ? 366 : 365;
        sum -= days1_sum;
   }

  return sum;
}


int diff_with_mktime(const string& date1, const string& date2){
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
   //printf("date1:  %d, %d, %d\n", year, month, day); 
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
   //printf("diff days between %s  and %s is %d\n", date1.c_str(), date2.c_str(), rc);
   return rc; 
 
}

TEST(didffdays, same_year_basic) {
    int days;
    days = diff_with_mktime("20110101", "20110105");
    EXPECT_EQ(days, 4);
    days = diff_days("20110101", "20110105");
    EXPECT_EQ(days, 4);
}

TEST(didffdays, different_year) {
    int days;
    int rc;
    rc = diff_with_mktime("20110101", "20120105");
    days = diff_days("20110101", "20120105");
    EXPECT_EQ(days, rc);
}

TEST(didffdays, leap_years) {
    int days;
    int rc;
    rc = diff_with_mktime("20110101", "20120505");
    days = diff_days("20110101", "20120505");
    EXPECT_EQ(days, rc);
}
TEST(didffdays, more_than_150_years) {
    int days;
    int rc;
    rc = diff_with_mktime("20110101", "22120505");
    days = diff_days("20110101", "22120505");
    EXPECT_EQ(days, rc);
}

TEST(didffdays, from_0010_years) {
    int days;
    days = diff_days("00110101", "00120101");
    EXPECT_EQ(days, 365);
    days = diff_days("00110101", "00120301");
    EXPECT_EQ(days, 365 + 31 + 29 );  
}










