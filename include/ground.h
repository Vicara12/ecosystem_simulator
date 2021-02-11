#ifndef GROUND_H_
#define GROUND_H_

#include <vector>


namespace gnd
{
    enum Item {Grass, Water, Tree};
    typedef std::vector<std::vector<Item>> Map;
    struct Point;
    struct TerrainCreature;
}


struct gnd::Point
{
    unsigned x, y;
};

struct gnd::TerrainCreature
{
    struct gnd::Point pos;
    char character;
};

#endif