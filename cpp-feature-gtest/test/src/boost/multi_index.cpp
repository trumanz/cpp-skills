#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>


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

struct id{}; struct name{}; struct age{};

typedef multi_index_container<
        employee,
        indexed_by<
           hashed_unique< tag<id>, BOOST_MULTI_INDEX_MEMBER(employee, int, id) >,
           ordered_non_unique< tag<name>, BOOST_MULTI_INDEX_MEMBER(employee, std::string, name) >,
           ordered_non_unique< tag<age>, BOOST_MULTI_INDEX_MEMBER(employee, int, age) > >
> employee_set;

static employee_set getEmployeeSet(){
   employee_set es;
   es.insert(employee(0,"A", 31) );
   es.insert(employee(1,"B", 32) );
   es.insert(employee(2,"C", 12) );
   es.insert(employee(3,"D", 31) );
   es.insert(employee(4,"E", 29) );
   return es;
}

TEST(boost_multi_index, unique_search_and_erase) {

   employee_set es = getEmployeeSet();
   employee_set::iterator it = es.get<id>().find(1);
   EXPECT_EQ(1, it->id);
   EXPECT_EQ("B", it->name);
   //erase it
   es.erase(it);
   //then, it dose not exist
   it =  es.get<id>().find(1);
   EXPECT_EQ(it, es.end());
}

TEST(boost_multi_index, group_by_age) {
   employee_set es = getEmployeeSet();
   std::string y; 
   for( boost::multi_index::index<employee_set, age>::type::iterator it = es.get<age>().begin();
              it != es.get<age>().end(); it = es.get<age>().equal_range(it->age).second ){
         std::cout  <<  it->name << "\n";
         y = y+ it->name;
   }
   EXPECT_EQ(y, "CEAB");
}

TEST(boost_multi_index, ordered_non_unique_search_between) {
   employee_set es;
   es.insert(employee(0,"A", 31));
   es.insert(employee(4,"E", 34 ));
   es.insert(employee(1,"F", 32 ));
   es.insert(employee(2,"F", 32 ));
   es.insert(employee(3,"F", 33 ));

   for(employee_set::iterator it = es.begin(); it != es.end(); it++) {
      //  printf("%d\n", it->id);
   }

   boost::multi_index::index<employee_set,age>::type::iterator  it1;
   boost::multi_index::index<employee_set,age>::type::iterator  it2;
   //search age in [32, 34)
   it1 =  es.get<age>().lower_bound(32);
   it2 =  es.get<age>().lower_bound(34);

   unsigned int count = 0;
   for(; it1 != it2; it1++) {
        count++;
        EXPECT_EQ(it1->name, "F");
   }

   EXPECT_EQ(3, count);
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


typedef multi_index_container<
        employee,
        indexed_by<
           sequenced<>,
           hashed_unique< tag<id>, BOOST_MULTI_INDEX_MEMBER(employee, int, id) >
        >
> employee_list;

TEST(boost_multi_index, sequenced_and_hashed_unique_discard_new_obj_if_the_same_key) {
   employee_list es;
   es.push_back(employee(0,"A", 1 ));
   es.push_back(employee(4,"B", 2 ));
   es.push_back(employee(4,"C", 3 ));//this will be disacrded
   es.push_back(employee(2,"C", 3 ));
   es.push_back(employee(3,"D", 4 ));
 
   unsigned int index = 1; 
   for(employee_list::iterator it = es.begin(); it != es.end(); it++) {
        EXPECT_EQ(it->age, index++);
   }
    

}
