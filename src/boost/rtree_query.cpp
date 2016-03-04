#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/index/rtree.hpp>

#include <inttypes.h>

class Rectangle
{
public:
  Rectangle(int id, unsigned int height, unsigned int width){
        this->id = id;
        this->heigth = height;
        this->width = width;
  }
  std::string tostr() {
      char buf[100];
      sprintf(buf, "id=%d, h=%lu, w=%lu", id, heigth, width);
      return buf;
  }
  int id;
  unsigned int heigth;
  unsigned int width;
};


std::list<Rectangle> getSome(int count){
    std::list<Rectangle> ret;
    for(int i = 0; i < count; i++) {
          ret.push_back(Rectangle(i,i,i));
    }
    return ret;
}

//http://www.boost.org/doc/libs/1_59_0/libs/geometry/doc/html/geometry/matrix.html
//typedef boost::geometry::model::point<uint64_t, 2, boost::geometry::cs::geographic > U64Point;
typedef boost::geometry::model::point<uint64_t, 2, boost::geometry::cs::cartesian > U64Point;
typedef std::pair<U64Point, int> Value;
typedef boost::geometry::index::rtree< Value, boost::geometry::index::linear<100> > RTree;


TEST(boost_rtree, spatial_query) {


   RTree tree;

   std::list<Rectangle> objs = getSome(100);
   for(std::list<Rectangle>::iterator it = objs.begin(); it != objs.end(); it++) {
      tree.insert(std::make_pair(U64Point(it->heigth, it->width),it->id));
   }

   std::vector<Value> results;


   boost::geometry::model::box<U64Point>  query_box(U64Point(0,0), U64Point(4,4));
   tree.query(boost::geometry::index::covered_by(query_box), std::back_inserter(results));   

   for(int i = 0 ; i < results.size(); i++){
       printf("%d\n", results[i].second);
   }   

}

