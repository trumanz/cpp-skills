#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>
#include <sstream>

#include "gen-cpp/me_types.h"

#include "thrift/transport/TFDTransport.h"
#include "thrift/transport/TBufferTransports.h"
#include "thrift/protocol/TJSONProtocol.h"

using namespace TGen;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;


TEST(thrift, serialize_to_json){
   Me me;
   me.__set_name("truman");
   boost::shared_ptr<TMemoryBuffer> trans(new TMemoryBuffer());
   TJSONProtocol json_proto(trans);
   me.write(&json_proto);
   printf("%s\n", trans->getBufferAsString().c_str());
}


#if 0
TEST(thrift, parse_json){

   Me me;
   int fd = open("./sample_data/me.json", O_RDONLY);
   boost::shared_ptr<TTransport>  file_trans(new TFDTransport(fd, TFDTransport::CLOSE_ON_DESTROY));
 
  
   TJSONProtocol json_proto(file_trans);

   me.read(&json_proto);

}
#endif
