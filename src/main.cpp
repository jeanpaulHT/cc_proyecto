#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
/*
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/foreach.hpp>
*/
#include "rtree.h"
#define NUM_IDX 4
/*
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
typedef bg::model::point<float, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef std::pair<box, int> value;
*/

void buildTreeFromIndex(){
  RTree<RouteNode>* rtree = new RTree<RouteNode>;
  for (int i=0; i<NUM_IDX; i++){
    std::ifstream idx;
    std::string idx_name = "idx/idx_green_" + std::to_string(i+1);
    idx.open(idx_name + ".csv", std::ios::in);
    int point_count {};
    std::string line, word, temp;
    while(idx >> temp){
      point_count++;
      std::vector<double> vect {};
      std::stringstream ss(temp);
      ss << std::fixed << std::setprecision(16);
      for(double i; ss >> i;){
        vect.push_back(i);    
        if (ss.peek() == ',')
          ss.ignore();
      }
      auto x = vect[0];
      auto y = vect[1];
      rtree->insert({x, y, x, y});
    }
    std::cout << "[build] Inserted " << point_count << " points from green_tripdata_" << i+1 << std::endl;
  }
  std::cout << "[build] Writing rtree.txt\n";
  std::ofstream resetFile ("rtree.txt", std::ios::trunc);
  resetFile.close();
  rtree->writeRTree(*rtree);
}

int main() {  
  buildTreeFromIndex();
  std::ifstream idx("rtree.txt");
  RTree<RouteNode>* project_rtree = new RTree<RouteNode>;  
  project_rtree->readRTree(*project_rtree, idx);
  idx.close();
  auto queryPoint = Point(-73.9858, 40.6857);
  MBR queryRegion(queryPoint, queryPoint);

  auto result = project_rtree->range_search(queryRegion);

  std::cout <<  "**=RANGE SEARCH (PROJECT IMPLEMENTATION)=**\n";

  for(auto i:result)
    std::cout<<i.start.x<<","<<i.start.y<<"\n";

  /*
  RTree<RouteNode> rtree;
  bgi::rtree<value, bgi::quadratic<16> > boostRtree;
  
  // El único dato relevante es el start point
  int i = 0;
  for(auto s: samples){ 
      rtree.insert({s[0], s[1], s[0], s[1]});
      auto p = point(s[0], s[1]);
      box b(p,p);
      boostRtree.insert(std::make_pair(b, ++i));
  }

  std::cout <<  "**=BOOST RTREE=**\n";

  std::vector<value> boostResults;
  boostRtree.query(bgi::intersects(box(r1, r2)), std::back_inserter(boostResults));
  BOOST_FOREACH(value const& v, boostResults)
      std::cout << bg::wkt<box>(v.first) << std::endl;
  */
  return 0;
}