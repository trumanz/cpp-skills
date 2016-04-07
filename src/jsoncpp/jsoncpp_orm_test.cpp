#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include <jsoncpp/json/json.h>
#include "jsoncpp_orm.h"

using namespace cpporm;

class Contact {
public:
    Contact(){}
    boost::shared_ptr<std::string> email;
    boost::shared_ptr<std::string> phone;
    void setORM(Mapper &mapper){
          mapper.orm("email", email);
          mapper.orm("phone", phone);
    }

};

class Skill {
public:
    boost::shared_ptr<std::string> language;
    boost::shared_ptr<int> grade;
    void setORM(Mapper &mapper) {
          mapper.orm("language", language);
          mapper.orm("grade", grade);
    }
};

class Me {
public:
    boost::shared_ptr<std::string> name;
    boost::shared_ptr<int> age;
    boost::shared_ptr<Contact> contact;
    boost::shared_ptr<std::list<std::string> > likes;
    boost::shared_ptr<std::list<Skill> > skills;
    boost::shared_ptr<int> not_existed;
    void setORM(Mapper &mapper){
          mapper.orm("name", name);
          mapper.orm("age", age);
          mapper.orm("contact", contact);
          mapper.orm("likes", likes);
          mapper.orm("skills", skills);
          mapper.orm("not_existed", not_existed);
    }
};

TEST(JsonROM, baisc){

     std::ifstream ifs("./sample_data/me.json",  std::ifstream::in);
     boost::shared_ptr<Me> me = JsonORM<Me>().get(ifs);
     ASSERT_EQ(*me->name ,"truman");
     ASSERT_EQ(*me->age ,30);

     ASSERT_EQ(me->not_existed.get() , (int*)NULL);

     boost::shared_ptr< std::list<std::string> > likes = me->likes;
     ASSERT_EQ(likes->size(), 2);
     ASSERT_EQ(likes->front(), "Batman");
     ASSERT_EQ(likes->back(), "Superman");

     boost::shared_ptr<Contact> contact = me->contact;
     ASSERT_EQ(*contact->email, "@com");
     ASSERT_EQ(*contact->phone, "123456");

     boost::shared_ptr<std::list<Skill> > skills = me->skills;
     ASSERT_EQ(skills->size(), 2);
     ASSERT_EQ(*skills->front().language, "c++");
     ASSERT_EQ(*skills->front().grade, 7);
     ASSERT_EQ(*skills->back().language, "R");
     ASSERT_EQ(*skills->back().grade, 0);
};

class Me2 {
public:
    boost::shared_ptr<int> not_existed;
    void setORM(Mapper &mapper){
          mapper.orm("not_existed", not_existed, false);
    }
   
};

TEST(JsonROM, not_exist){
   
     try {
       std::ifstream ifs("./sample_data/me.json",  std::ifstream::in);
       boost::shared_ptr<Me2> me = JsonORM<Me2>().get(ifs);
     }  catch ( CppOrmNotFoundException e) {
             ASSERT_EQ(std::string(".not_existed not found"), e.what());
     }
};


class Skill2 {
public:
    boost::shared_ptr<std::string> language;
    boost::shared_ptr<int> grade;
    void setORM(Mapper &mapper) {
          mapper.orm("language_not_exist", language, false);
    }
};

class Me3 {
public:
    boost::shared_ptr<std::string> name;
    boost::shared_ptr<std::list<Skill2> > skills;
    void setORM(Mapper &mapper){
          mapper.orm("name", name);
          mapper.orm("skills", skills);
    }
};
TEST(JsonROM, not_exist2){
     try {
       std::ifstream ifs("./sample_data/me.json",  std::ifstream::in);
       boost::shared_ptr<Me3> me = JsonORM<Me3>().get(ifs);
     }  catch ( CppOrmNotFoundException e) {
             printf("%s\n", e.what());
             ASSERT_EQ(std::string(".skills[0].language_not_exist not found"), e.what());
     }
};
