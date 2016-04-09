#include "jsoncpp_orm.h"
using namespace cpporm;

void Mapper::get(Json::Value json, int* e){
      e[0] =  json.asInt();
}



void Mapper::get(Json::Value json, std::string* e){
      e[0] =  json.asString();
}



