#ifndef _BLOCKING_QUEUE_
#ifndef _BLOCKING_QUEUE_

#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <iostream>

#include <boost/atomic.hpp>
#include <boost/thread/locks.hpp> 
#include <boost/thread/lock_guard.hpp> 


template<T>
class BlockingQueue {

public:
     void offer(T e, int timeout) {
        int c;
        {
           boost::lock_gurard<boost::mutext> lock(putLock);

           while(count.get() == capacity) {
               timeout = notFull.wait(timeout);
           } 
           queue.push(e);
           c = count.getAndIncrement();
           if(c + 1 < capacity) notFull.signal();
        }
        if( c == 0) signalNotEmpty(); 
     }
     T  poll(int timeout) {
          int c;
          T e;
          {
              boost::lock_gurard<boost::mutext> lock(takeLock);
              while(count.get() == 0){
                  timeout = notEmpty.wait(timeout); 
              }
              e = queue.pop();
              c = count.getAndDecrement();
              if(c > 1) notEmpty.signal();
              takeLock.unlock();
           }
          
           if(c == capacity)  signalNotFull();
           return e;
     }
     
private:
    

     private void signalNotEmpty(){
          takeLock.lock();
          notEmpty.signal();
          takeLock.unlock();
     }
     private void signalNotFull(){
          putLock.lock();
          notFull.signal();
          putLock.unlock();
     }

     boost::lockfree::queue<T>  queue(capacity);
     boost::atomic_int count;

     boost::mutex takeLock;
     Condition notEmpty = Condiation(takeLock);
     
     boost::mutex putLock;
     Condition notFull = Condiation(putLock);
};


#endif
