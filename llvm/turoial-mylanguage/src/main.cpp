#include <iostream>
#include <sstream>
#include <vector>
#include "parser.h"
#include <log4cplus/log4cplus.h>

void initLog() {

    static log4cplus::Initializer initializer;
    log4cplus::SharedAppenderPtr  appenderPtr(new log4cplus::ConsoleAppender(false, true));
    std::string pattern = "%d{%H:%M:%s} %-5p - %m [%M %F:%L] %n";
//std::string pattern = "%H:%M:%S - %m[%l]%n";
    appenderPtr->setLayout(std::unique_ptr<log4cplus::Layout>(new log4cplus::PatternLayout(pattern)));
    log4cplus::Logger logger = log4cplus::Logger::getRoot();
    logger.setLogLevel(log4cplus::DEBUG_LOG_LEVEL);
    logger.addAppender(appenderPtr);

    LOG4CPLUS_INFO_FMT(logger, "%s", __PRETTY_FUNCTION__);
    LOG4CPLUS_INFO_FMT(logger, "this is test");
}

int main() {
    //std::cout << "Hello, World!" << std::endl;
    initLog();
    Parser parser;
    //parser.parse("def foo(x y) x+y+1 y;");
    parser.parse("1+2;");
    parser.parse("def foo(a b) a*a + 2*a*b + b*b;");
    return 0;

}
