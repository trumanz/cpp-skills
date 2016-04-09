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
#include <boost/shared_ptr.hpp>
#include <stdexcept>

namespace  cpporm {

class CppOrmNotFoundException : public std::runtime_error
{
   public:
    CppOrmNotFoundException(std::string const& node_name)
        : runtime_error(node_name + " not found")
    { }
    
    CppOrmNotFoundException(const CppOrmNotFoundException& e, std::string const& node_name)
        : runtime_error(node_name + e.what())

    { }
    
};

//TODO is_encode not used, just for decode
class Mapper {
public:
    Mapper(Json::Value json, bool is_encode){
        this->json = json;
        this->is_encode = is_encode;
    }

    template<typename T>
    void orm(const std::string& name, T& v){
        //printf("filed %s\n", name.c_str());
         Json::Value jv = json[name];
         if(!jv.isNull()) {
             try {
                get(jv, &v);
             } catch (CppOrmNotFoundException e) {
                throw CppOrmNotFoundException(e, std::string(".") + name);
             }
         } else  {
              throw CppOrmNotFoundException(std::string(".") + name);
         }
    }

    template<typename T>
    void orm(const std::string& name, boost::shared_ptr<T>& v){
        //printf("filed %s\n", name.c_str());
         Json::Value jv = json[name];
         if(!jv.isNull()) {
             T  e;
             orm(name, e);
             v = boost::shared_ptr<T>(new T(e));
         } 
    }

private: //for std container type
    template<typename T>
    void get(Json::Value json, std::list<T>* e){
            //printf("list\n");
            for(int i = 0; i  < json.size(); i++) {
                 try { 
                    T tmp;
                    get(json[i], &tmp);
                    e->push_back(tmp);
                 } catch  (CppOrmNotFoundException e) {
                    char buf[20];
                    snprintf(buf, 19, "[%d]", i);
                    throw CppOrmNotFoundException(e, buf);
                 }
            }
    }
private: // for class type
    template<typename T>
    void get(Json::Value json, T* e){
       Mapper mapper(json, false);
       e->setORM(mapper);
    } 
private:  //for basic type
    void get(Json::Value json, int*);
    void get(Json::Value json, std::string *);

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
    boost::shared_ptr<T> get(std::istream &is){
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
         return boost::shared_ptr<T>(e);
    }
};


}
