#ifndef TERRAIN_H_
#define TERRAIN_H_


#include <vector>


namespace gnd
{
    enum Item {Soil, Water, Tree};  // elements that can be found in the terrain
    typedef std::vector<std::vector<Item>> Map;
}


class Terrain
{
public:

    Terrain ();

private:

};

#endif