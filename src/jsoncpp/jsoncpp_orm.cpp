#include "jsoncpp_orm.h"


int Mapper::get(Json::Value json, int*){
      return json.asInt();
}



std::string Mapper::get(Json::Value json, std::string*){
      return json.asString();
}



