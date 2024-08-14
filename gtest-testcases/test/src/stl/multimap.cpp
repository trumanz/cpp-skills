#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <map>
#include <string>




TEST(stl_multimap, lower_upper_bound) {

    std::multimap<int, std::string> id_string_multimap;
  
    id_string_multimap.insert(std::make_pair(0, "0a"));
    id_string_multimap.insert(std::make_pair(1, "1a"));
    id_string_multimap.insert(std::make_pair(1, "1b"));
    id_string_multimap.insert(std::make_pair(1, "1c"));
    id_string_multimap.insert(std::make_pair(1, "1d"));
    id_string_multimap.insert(std::make_pair(2, "2a"));
    id_string_multimap.insert(std::make_pair(2, "2b"));

 
    std::multimap<int, std::string>::iterator  lower_it = id_string_multimap.lower_bound(1);
    std::multimap<int, std::string>::iterator  upper_it = id_string_multimap.upper_bound(1);


    ASSERT_EQ(lower_it->second, "1a"); lower_it++;
    ASSERT_EQ(lower_it->second, "1b"); lower_it++;
    ASSERT_EQ(lower_it->second, "1c"); lower_it++;
    ASSERT_EQ(lower_it->second, "1d"); lower_it++;

    ASSERT_EQ(lower_it, upper_it); 

    ASSERT_EQ(upper_it->second, "2a");
}
