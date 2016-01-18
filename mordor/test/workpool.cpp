#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include "mordor/workerpool.h"
#include <boost/bind.hpp>
#include <boost/utility.hpp>
#include <boost/thread.hpp>

using namespace std;
using namespace Mordor;


class Worker : boost::noncopyable {
public:
    int count;
    boost::thread::id id;
    Worker() {
       count = 0;
    }
    void run(string message){
       int i = 5;
       while(i--) {
           //printf("%s\n", message.c_str());
           count++;
           Scheduler::yield();
       }
       id = boost::this_thread::get_id();
   }
   
};

TEST(WorkPool, multi_woker_in_one_thread) {
 
   boost::thread::id id =  boost::this_thread::get_id();
   Worker w1;
   Worker w2;
   Worker w3;

   {
     //pool will call stop on deconstructor funciton 
     //create one thread for the pool
     size_t threads = 1;
     bool useCaller = false;
     WorkerPool  pool(threads, useCaller);
     pool.schedule(boost::bind(&Worker::run, &w1, "xxx"));
     pool.schedule(boost::bind(&Worker::run, &w2, "yyy"));
     pool.schedule(boost::bind(&Worker::run, &w3, "zzz"));
   }

   EXPECT_EQ(w1.count,5);
   EXPECT_EQ(w2.count,5);
   EXPECT_EQ(w3.count,5);


   EXPECT_NE(id ,w1.id);
   EXPECT_EQ(w2.id ,w1.id);
   EXPECT_EQ(w2.id ,w3.id);
}
