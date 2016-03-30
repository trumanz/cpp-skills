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
   // std::list<Skill> skills;
    void setORM(Mapper &mapper){
          mapper.set("name", name);
          //mapper.set("skillList",  skills);
    }
};

TEST(JsonROM, baisc){
     Me* me = JsonORM<Me>().get("{\"name\": \"truman\"}");
     ASSERT_EQ(me->name ,"truman");
};

