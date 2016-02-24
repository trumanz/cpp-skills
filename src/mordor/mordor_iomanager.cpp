#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#include "gtest/gtest.h"

#include "mordor/iomanager.h"
#include "mordor/log.h"
#include "mordor/socket.h"
#include "mordor/streams/socket.h"
#include "mordor/streams/transfer.h"


#include <boost/bind.hpp>
#include <boost/utility.hpp>
#include <boost/thread.hpp>
#include <boost/thread/barrier.hpp>

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
          MORDOR_LOG_DEBUG(Log::root()) <<  " read on  "  << this->getReadFd() ;
    }
    void write(size_t num){
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
//   Log::root()->level(Log::TRACE);
//   Log::root()->addSink( LogSink::ptr(new StdoutLogSink));
   //IOManager(size_t threads = 1, bool useCaller = true, bool autoStart = true);
   IOManager iomanager;
   const size_t conn_num = 5;
   std::vector<boost::shared_ptr<Conn> > conns; 
   conns.reserve(conn_num);

   //register event
   for(size_t i = 0 ; i < conn_num; i++){
       boost::shared_ptr<Conn> conn(new Conn);
       iomanager.registerEvent(conn->getReadFd(), IOManager::READ,  boost::bind(&Conn::read, conn));
       conns.push_back(conn);
   }

   //write 
   for(size_t i = 0; i < conns.size(); i++) {
      MORDOR_LOG_DEBUG(Log::root()) <<  " write on for  "  << conns[i]->getReadFd();
      conns[i]->write(100);
   }

    iomanager.dispatch();
    iomanager.stop();

   for(size_t i = 0; i < conns.size(); i++) {
      ASSERT_EQ(boost::this_thread::get_id() , conns[i]->tid);
      ASSERT_EQ(100, conns[i]->read_count);
   }
}


class TCPServer {
public:
     TCPServer(const char *ip, const unsigned short port, unsigned int conn_num) : 
                 address(new IPv4Address(ip, port)) {
              this->conn_num = conn_num;
     }

     void start(boost::barrier *barrier){
         bthread = boost::thread(&TCPServer::threadEntry, this, barrier);
     }
     void threadEntry(boost::barrier *barrier){
          printf("start\n");
          //IOManager must be constructor in this thread context, so that Schedule::getThois could work on startListen
          this->iomanager = boost::shared_ptr<IOManager>(new IOManager());
          Socket::ptr listen_socket;
          listen_socket  = address->createSocket(*(iomanager.get()), SOCK_STREAM);
          listen_socket->bind(address);
          iomanager->schedule(boost::bind(&TCPServer::startListen, this, listen_socket, barrier));
          iomanager->dispatch();
     }
    void wait_stop(){
        bthread.join();
    }
private:
    Address::ptr address;
    boost::thread bthread;
    boost::shared_ptr<IOManager> iomanager;
    unsigned int conn_num;

    void handleConnect(Socket::ptr socket) {
        char buf[10];
        while(1) { 
            //receive will switch the contex to epoll fiber
               size_t len = socket->receive(buf, sizeof(buf));
               if(len == 0) {
                  printf("receive 0, closed by remote\n");
                  break;
               }
               socket->send(buf, len);
         }
         socket->shutdown();
   }

   void startListen(Socket::ptr listen_socket, boost::barrier* barrier){
     listen_socket->listen();
     if(barrier) barrier->wait();
     while(conn_num--) {
            //accept will switch the contex to epoll fiber
            Socket::ptr socket = listen_socket->accept();
            Scheduler::getThis()->schedule(boost::bind(&TCPServer::handleConnect, this, socket));
     }
     listen_socket->shutdown();
   }

};
TEST(IOManager, tcp_server) {
    char buf[10];
//    Log::root()->level(Log::TRACE);
//    Log::root()->addSink( LogSink::ptr(new StdoutLogSink));

    boost::barrier barrier(2);
    unsigned short port = 31234;
  
    TCPServer tcpserver("0.0.0.0", port, 2);
    tcpserver.start(&barrier);

    barrier.wait();


    int fd1 = socket(AF_INET, SOCK_STREAM, 0);
    int fd2 = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in echoserver;
    memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
    echoserver.sin_family = AF_INET;                  /* Internet/IP */
    echoserver.sin_addr.s_addr = inet_addr("127.0.0.1");  /* IP address */
    echoserver.sin_port = htons(port);       /* server port */


    //
    assert(0 == connect(fd1, (struct sockaddr *) &echoserver, sizeof(echoserver)));
    assert(0 == connect(fd2, (struct sockaddr *) &echoserver, sizeof(echoserver)));

    // 
    send(fd1, "xxxxxxxx", 5, 0);
    send(fd2, "xxxxxxxx", 5, 0);
   
    //fd2's echo would not blocked by fd1's  
    ASSERT_EQ(5 , recv(fd2, buf, 10, 0));
    ASSERT_EQ(5 , recv(fd1, buf, 10, 0));
    close(fd1);
    close(fd2);

    tcpserver.wait_stop();

}
