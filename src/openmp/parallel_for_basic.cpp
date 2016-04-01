#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <cstdio>
#include <unistd.h>
#include <omp.h>

using namespace boost::posix_time;


 
TEST(openmp, paralle_for_basic) 
{
    int count = 128;
    int largest_task = 0;

    printf("thread max %d\n", omp_get_max_threads());

    for (int i = count - count/omp_get_max_threads() ; i < count;i++) {
          largest_task = largest_task + i;
    }
    printf("largest_task: %d\n", largest_task);


    ptime t1 =  microsec_clock::local_time();
    #pragma omp parallel for
    for (int i = 0; i < count;i++) {
           usleep(i*1000);
    }
    time_duration  duration =   microsec_clock::local_time() - t1;

    printf("duration: %d\n", duration.total_milliseconds());
    ASSERT_NEAR(duration.total_milliseconds() , largest_task, 100);

}

