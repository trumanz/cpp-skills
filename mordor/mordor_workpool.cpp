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
    boost::thread::id tid;
    Worker() {
       count = 0;
    }
    void run(int count){
       while(count--) {
           //printf("%s\n", message.c_str());
           this->count++;
           Scheduler::yield();
       }
       this->tid = boost::this_thread::get_id();
   }
};

TEST(WorkPool, multi_woker_in_one_thread) {
 
   boost::thread::id tid =  boost::this_thread::get_id();
   size_t worker_num = 1000;
   std::vector<boost::shared_ptr<Worker> > workers;
   workers.reserve(worker_num);
   {
     //pool will call stop on deconstructor funciton 
     //create one thread for the pool
     size_t threads = 1;
     bool useCaller = false;
     WorkerPool  pool(threads, useCaller);
     for(size_t i = 0 ; i  <  worker_num; i++){
         boost::shared_ptr<Worker>  worker(new Worker());
         workers.push_back(worker);
         pool.schedule(boost::bind(&Worker::run, worker, i));
     }
   }

   for(size_t i = 0 ; i <  workers.size(); i++){
           EXPECT_EQ(workers[i]->count, i);
           EXPECT_NE(workers[i]->tid, tid);
           if( i + 1 < workers.size()) EXPECT_EQ(workers[i]->tid, workers[i+1]->tid);
   }

}


TEST(WorkPool, multi_woker_in_multi_thread) {
 
   boost::thread::id tid =  boost::this_thread::get_id();
   size_t worker_num = 1000;
   std::vector<boost::shared_ptr<Worker> > workers;
   workers.reserve(worker_num);
   {
     //pool will call stop on deconstructor funciton 
     //create one thread for the pool
     size_t threads = 2;
     bool useCaller = false;
     WorkerPool  pool(threads, useCaller);
     for(size_t i = 0 ; i  <  worker_num; i++){
         boost::shared_ptr<Worker>  worker(new Worker());
         workers.push_back(worker);
         pool.schedule(boost::bind(&Worker::run, worker, i));
     }
   }

   set<boost::thread::id> tids;
   for(size_t i = 0 ; i <  workers.size(); i++){
           EXPECT_EQ(workers[i]->count, i);
           EXPECT_NE(workers[i]->tid, tid);
           tids.insert(workers[i]->tid);
   }
   EXPECT_EQ(tids.size(), 2);

}


