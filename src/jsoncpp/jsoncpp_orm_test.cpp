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

#if 0
class Skill : public JsonObj {
public:
    std::string language;
protected:
    void setORM(Mapper &mapper){
          mapper.orm("language", language);
    }
};
#endif

class Contact {
public:
    Contact(){}
    std::string email;
    std::string phone;
    void setORM(Mapper &mapper){
          mapper.orm("email", email);
          mapper.orm("phone", phone);
    }

};

class Skill {
public:
    std::string language;
    int grade;
    void setORM(Mapper &mapper) {
          mapper.orm("language", language);
          mapper.orm("grade", grade);
    }
};

class Me {
public:
    std::string name;
    int age;
    Contact contact;
    std::list<std::string> likes;
    std::list<Skill> skills;
    void setORM(Mapper &mapper){
          mapper.orm("name", name);
          mapper.orm("age", age);
          mapper.orm("contact", contact);
          mapper.orm("likes", likes);
          mapper.orm("skills", skills);
    }
};

TEST(JsonROM, baisc){
     std::ifstream ifs("./sample_data/me.json",  std::ifstream::in);
     Me* me = JsonORM<Me>().get(ifs);
     ASSERT_EQ(me->name ,"truman");
     ASSERT_EQ(me->age ,30);
   
     std::list<std::string> likes = me->likes;
     ASSERT_EQ(likes.size(), 2);
     ASSERT_EQ(likes.front(), "Batman");
     ASSERT_EQ(likes.back(), "Superman");


     Contact contact = me->contact;
     ASSERT_EQ(contact.email, "@com");
     ASSERT_EQ(contact.phone, "123456");

     std::list<Skill> skills = me->skills;
     ASSERT_EQ(skills.size(), 2);
     ASSERT_EQ(skills.front().language, "c++");
     ASSERT_EQ(skills.front().grade, 7);
     ASSERT_EQ(skills.back().language, "R");
     ASSERT_EQ(skills.back().grade, 0);

};

