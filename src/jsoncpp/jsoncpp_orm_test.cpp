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
          mapper.set("language", language);
    }
};
#endif

class Me {
public:
    std::string name;
    int age;
    std::list<std::string> likes;
   // std::list<Skill> skills;
    void setORM(Mapper &mapper){
          mapper.set("name", name);
          mapper.set("age", age);
          mapper.set("likes", likes);
         // mapper.set("skillList",  skills);
    }
};

TEST(JsonROM, baisc){
     std::ifstream ifs("./sample_data/me.json",  std::ifstream::in);
     Me* me = JsonORM<Me>().get(ifs);
     ASSERT_EQ(me->name ,"truman");
     ASSERT_EQ(me->age ,30);
     ASSERT_EQ(me->likes.size(), 2);
     ASSERT_EQ(me->likes.front(), "Batman");
     ASSERT_EQ(me->likes.back(), "Superman");
};

