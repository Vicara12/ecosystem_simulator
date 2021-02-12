#include "ground.h"

#include <math.h>


float gnd::distance (const Point &p1, const Point &p2)
{
    return sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
}


gnd::Point::Point (float x_, float y_) : x(x_), y(y_)
{
    //
}

gnd::TerrainCreature::TerrainCreature (struct gnd::Point p, char c) : 
    pos(p),
    character(c)
{
    //
}