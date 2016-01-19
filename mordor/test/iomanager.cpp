#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "gtest/gtest.h"
#include "mordor/iomanager.h"
#include "mordor/log.h"
#include <boost/bind.hpp>
#include <boost/utility.hpp>
#include <boost/thread.hpp>

#include <event2/util.h>

using namespace std;
using namespace Mordor;

class Conn : boost::noncopyable{
public:
    Conn(){
       read_count = 0;
       assert(socketpair(AF_UNIX, SOCK_STREAM, 0, this->sockets) == 0);
    }
    ~Conn(){
       close(sockets[0]);
       close(sockets[1]);
    }
    boost::thread::id tid;
    size_t read_count;
    void read(){
          char buf[100];
          read_count += ::read(sockets[0], buf, sizeof(buf));
          this->tid = boost::this_thread::get_id();
    }
    void write(size_t num){
         MORDOR_LOG_DEBUG(Log::root()) <<  " read on  "  << this->getReadFd() ;
         std::vector<char> buf(num, 'a');
         ::write(sockets[1], buf.data(), buf.size());
    }
    int getReadFd() const {
       return sockets[0];
    }
private:
    int sockets[2];
    
};

//on linux Mordor::IOManager user epoll
TEST(IOManager, event) {
   Log::root()->level(Log::TRACE);
   Log::root()->addSink( LogSink::ptr(new StdoutLogSink));
   //IOManager(size_t threads = 1, bool useCaller = true, bool autoStart = true);
   IOManager iomanager;
   const size_t conn_num = 5;
   std::vector<boost::shared_ptr<Conn> > conns; 
   conns.reserve(conn_num);
   for(size_t i = 0 ; i < conn_num; i++){
       boost::shared_ptr<Conn> conn(new Conn);
       iomanager.registerEvent(conn->getReadFd(), IOManager::READ,  boost::bind(&Conn::read, conn));
       conns.push_back(conn);
   }
   for(size_t i = 0; i < conns.size(); i++) {
      MORDOR_LOG_DEBUG(Log::root()) <<  " write on for  "  << conns[i]->getReadFd();
      conns[i]->write(100);
   }
   sleep(2);

   for(size_t i = 0; i < conns.size(); i++) {
      MORDOR_LOG_DEBUG(Log::root()) <<  " write on for  "  << conns[i]->getReadFd();
      conns[i]->write(100);
   }

   iomanager.stop();

   for(size_t i = 0; i < conns.size(); i++) {
      ASSERT_EQ(boost::this_thread::get_id() , conns[i]->tid);
      ASSERT_EQ(100, conns[i]->read_count);
   }
}

