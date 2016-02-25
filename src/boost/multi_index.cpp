#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>

using namespace std;
using boost::multi_index_container;
using namespace boost::multi_index;


struct employee
{
  int id;
  std::string name;
  int age;
  
  employee(int id_, std::string name_, int age_): id(id_), name(name_), age(age_) {}

};


struct id{};
struct name{};
struct age{};

typedef multi_index_container<
        employee,
        indexed_by<
           hashed_unique< tag<id>, BOOST_MULTI_INDEX_MEMBER(employee, int, id) >,
           ordered_non_unique< tag<name>, BOOST_MULTI_INDEX_MEMBER(employee, std::string, name) >,
           ordered_non_unique< tag<age>, BOOST_MULTI_INDEX_MEMBER(employee, int, age) > >
> employee_set;


TEST(boost_multi_index, hashed_unique_search) {
   employee_set es;
   es.insert(employee(0,"A", 31));
   es.insert(employee(1,"B", 32 ));
   es.insert(employee(2,"C", 32 ));

   employee_set::iterator it;
   it =   es.get<id>().find(1);
   EXPECT_EQ(1, it->id);
   EXPECT_EQ("B", it->name);

   it =   es.get<id>().find(3);
    
   EXPECT_EQ(it, es.end());

}

TEST(boost_multi_index, hashed_unique_descard_new_obj_if_the_same_key) {
   employee_set es;
   
   es.insert(employee(0,"Joe", 31));
   es.insert(employee(0,"Jack", 32 ));

   EXPECT_EQ(1,  es.size());
  
   employee_set::iterator it = es.begin();
 
   EXPECT_EQ(0,  it->id);
   EXPECT_EQ("Joe",  it->name);
   EXPECT_EQ(31,  it->age);

}


