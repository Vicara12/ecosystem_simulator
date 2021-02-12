#ifndef GROUND_H_
#define GROUND_H_

#include <vector>


namespace gnd
{
    enum Item {Grass, Water, Tree};
    typedef std::vector<std::vector<Item>> Map;
    struct Point;
    struct TerrainCreature;
    float distance (const Point &p1, const Point &p2);
}


struct gnd::Point
{
    float x, y;

    Point (float x_, float y_);
};

struct gnd::TerrainCreature
{
    struct gnd::Point pos;
    char character;

    TerrainCreature (struct gnd::Point p, char c);
};

#endif