#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include <jsoncpp/json/json.h>


class Mapper {
public:
    Mapper(Json::Value json, bool is_encode){
        this->json = json;
        this->is_encode = is_encode;
    }
    void set(std::string name, std::string& v){
         if(!is_encode) {
            v =  json[name].asString();
         }
    }
#if 0
    template<typename T>
    public void set(std::string name, std::list<T>& v){
         for(int i = 0; i < root[name].size(); i++){
             T x;
             Mapper mapper(root[i], is_encode);
             x->setORM(mapper);
             v->push_back(x);
         }
    }
#endif
private:
    Json::Value json;
    bool is_encode;
};

#if 0
class JsonObj{
protected:
   virtual void setORM(Mapper &mapper) const = 0;
};
#endif


template<typename T>
class JsonORM{
public:
    T* get(std::string json){
         T* e = new T;
         std::stringstream ss;
         ss << json;
         Json::Value root;
         Json::Reader reader;
         bool parsingSuccessful = reader.parse(ss, root);
    
         if(!parsingSuccessful) {
           printf("Failed to parse, %s\n", reader.getFormatedErrorMessages().c_str());
         } else {
             Mapper mapper(root, false);
             e->setORM(mapper);
         }
         return e;

    }
};


