#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <cstdio>
#include <unistd.h>
#ifdef _OPENMP
#include <omp.h>
#endif

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


    

    #ifdef _OPENMP
    omp_lock_t write_lock;
    omp_init_lock(&writelock);
    #endif

    int count_sum = 0;

    #pragma omp parallel for
    for (int i = 0; i < count;i++) {
           usleep(i*1000);
           #ifdef _OPENMP
           omp_set_lock(&writelock);
           count_sum++;
           omp_unset_lock(&writelock);
           #endif
    }

    #ifdef _OPENMP
    ASSERT_EQ(count_sum, count);
    #endif


    time_duration  duration =   microsec_clock::local_time() - t1;

    printf("duration: %ld\n", duration.total_milliseconds());
    ASSERT_NEAR(duration.total_milliseconds() , largest_task, 100);

}

