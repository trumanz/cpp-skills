#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>
#include <sstream>
#include <fstream>      // std::ifstream
#include <jsoncpp/json/json.h>
#include <assert.h>
#include <list>



//TODO is_encode not used, just for decode
class Mapper {
public:
    Mapper(Json::Value json, bool is_encode){
        this->json = json;
        this->is_encode = is_encode;
    }

    template<typename T>
    T get(Json::Value json, T* dummy){
       T e;
       Mapper mapper(json, false);
       e.setORM(mapper);
       return e;
    } 

    int get(Json::Value json, int*);
    std::string get(Json::Value json, std::string *);


    template<typename T>
    void orm(std::string name, T& v){
         v = get(json[name], &v);
    }

    template<typename T>
    void orm(std::string name, std::list<T>& v){
            for(int i = 0; i  < json[name].size(); i++) {
                 T e = get(json[name][i], &e);
                 v.push_back(e);
            }
    }

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
    T* get(std::istream &is){
         T* e = new T;
         Json::Value root;
         Json::Reader reader;
         bool parsingSuccessful = reader.parse(is, root);
    
         if(!parsingSuccessful) {
           printf("Failed to parse, %s\n", reader.getFormatedErrorMessages().c_str());
         } else {
             Mapper mapper(root, false);
             e->setORM(mapper);
         }
         return e;
    }
};


