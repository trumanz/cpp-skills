#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include <jsoncpp/json/json.h>


TEST(jsoncpp, parse) {

   std::ifstream ifs("./sample_data/me.json",  std::ifstream::in);

   Json::Value root;
   Json::Reader reader;
   bool parsingSuccessful = reader.parse(ifs, root);

   if(!parsingSuccessful) {
        printf("Failed to parse, %s\n", reader.getFormatedErrorMessages().c_str());
   }
   ASSERT_EQ(parsingSuccessful, true);

   ASSERT_EQ("truman", root.get("name", "thisDefaultValue").asString());
  
   ASSERT_EQ(4, root["work_experience"].size());
   ASSERT_EQ("Eastsoft", root["work_experience"][0].asString());
   ASSERT_EQ("EMC",      root["work_experience"][3].asString());

}
#if 0
TEST(jsoncpp, parse_not_exist) {

   std::stringstream ss;
   ss << getExampleJson();
   Json::Value root;
   Json::Reader reader;
   //printf("parse, \n%s\n", ss.str().c_str());
   bool parsingSuccessful = reader.parse(ss, root);

   if(!parsingSuccessful) {
        printf("Failed to parse, %s\n", reader.getFormatedErrorMessages().c_str());
   }
   ASSERT_EQ(parsingSuccessful, true);

   std::string encoding = root.get("encoding2", "thisDefaultValue").asString();
   ASSERT_EQ(encoding, "thisDefaultValue");
  
   Json::Value plugins = root["plug-ins2"];
   ASSERT_EQ(true, plugins.empty());

}



TEST(jsoncpp, path_parse) {

   std::stringstream ss;
   ss << getExampleJson();
   Json::Value root;
   Json::Reader reader;
   //printf("parse, \n%s\n", ss.str().c_str());
   bool parsingSuccessful = reader.parse(ss, root);

   if(!parsingSuccessful) {
        printf("Failed to parse, %s\n", reader.getFormatedErrorMessages().c_str());
   }
   ASSERT_EQ(parsingSuccessful, true);

   Json::Path p("indent.length");
   Json::Value v = p.resolve(root);
   ASSERT_EQ(v.asInt(), 3);

   Json::Path p2("indent.length_xx");
   v = p2.resolve(root);
   EXPECT_EQ(true, v.isNull());

   Json::Path p3("indent.length_xx.yy");
   v = p3.resolve(root);
   EXPECT_EQ(true, v.isNull());
}



TEST(jsoncpp, path_list) {

   std::stringstream ss;
   ss << getExampleJson();
   Json::Value root;
   Json::Reader reader;
   //printf("parse, \n%s\n", ss.str().c_str());
   bool parsingSuccessful = reader.parse(ss, root);

   if(!parsingSuccessful) {
        printf("Failed to parse, %s\n", reader.getFormatedErrorMessages().c_str());
   }
   ASSERT_EQ(parsingSuccessful, true);

   Json::Path p("plug-ins");
   Json::Value v = p.resolve(root);
   ASSERT_EQ(v.size(), 3);

   Json::Path p2("plug-ins_xx");
   v = p2.resolve(root);
   ASSERT_EQ(true,  v.isNull());

   Json::Path p3("plug-ins_xx");
   v = p3.resolve(root, "[]");
   ASSERT_FALSE(v.isNull());
   ASSERT_EQ(0,  v.size());

}

#endif

