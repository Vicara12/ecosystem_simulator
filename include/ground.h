#ifndef GROUND_H_
#define GROUND_H_

#include <vector>


namespace gnd
{
    enum Item {Soil, Water, Tree};  // elements that can be found in the terrain
    typedef std::vector<std::vector<Item>> Map;
    struct Point;
}


struct gnd::Point
{
    unsigned x, y;
};

#endif