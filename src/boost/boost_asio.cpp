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



#include <boost/bind.hpp>
#include <boost/utility.hpp>
#include <boost/thread.hpp>
#include <boost/thread/barrier.hpp>

#include <event2/util.h>

#if 0
class TCPEchoServer {
public:
    
     

    TCPEchoServer(std::string ip, unsigned int port){
       
    }
    void start(){
        try{
            tcp::acceptor acceptor(io_service, tcp::endpint(tcp::v4(), 13));
            acceptor.async_accept(new_connection



            while(1){
                 tcp::socket socket(io_service);
                 acceptor.accept(socket);
                 std::string message = make_daytime_string();
                 boost::system::error_code ignored_error;
                 boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
            }
        } catch(std::exception &e){
            std::cerr << e.what() << std::endl;
        }
    }
private:
    boost::asio::io_service io_service;    
}

int mian(){
    unsigned short port = 30000;
    TCPEchoServer server = new TCPEchoServer("0.0.0.0", port);
    server.start();

        
   
}
#endif
