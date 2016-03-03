#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/composite_key.hpp>


using namespace std;
using boost::multi_index_container;
using namespace boost::multi_index;


class Flight
{
public:
  Flight(int id, time_t timestamp, const char *location){
        this->id = id;
        this->timestamp = timestamp;
        this->location = location;
  }
  int id;
  time_t timestamp;
  std::string location;
};

struct id{};
struct timestamp{};
struct timelocation{};

struct timeLocation_key : composite_key <
    Flight,
    BOOST_MULTI_INDEX_MEMBER(Flight, time_t,  timestamp),
    BOOST_MULTI_INDEX_MEMBER(Flight, std::string,  location)
>{};

typedef multi_index_container<
        Flight,
        indexed_by<
           hashed_unique< tag<id>, BOOST_MULTI_INDEX_MEMBER(Flight, int, id) >,
           ordered_non_unique< tag<timelocation>, timeLocation_key>
        >
> FlightSet;

TEST(boost_multi_index, combin_search) {
   FlightSet fs;
   fs.insert(Flight(0, 10, "A") );
   fs.insert(Flight(1, 20, "B") );
   fs.insert(Flight(2, 20, "B") );
   fs.insert(Flight(2, 40, "D") );

   boost::multi_index::index<FlightSet, timelocation >::type::iterator it0, it1;
   boost::tie(it0, it1) = fs.get<timelocation>().equal_range(boost::make_tuple(20, "B"));

   for(; it0 != it1; it0++){
       printf("%d\n", it0->id);
   }

}

