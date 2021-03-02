#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <string>
#include <list>
#include <vector>
#include "ground.h"


class Terrain
{
public:

    // generate random map with the specified dimensions
    // terrain complexity indicates the number of cells in the perlin noise map
    // qty of terrain gives an approximation of the proportion of the terrain
    // that will be covered by grass, it must be an integer between 0 and 100
    // qty of trees indicates the tree density in the map, it's also a number
    // between 0 and 100
    Terrain (unsigned width,
             unsigned height,
             unsigned terrain_complexity = 3,
             unsigned qty_of_terrain = 60,
             unsigned qty_of_trees = 40,
             unsigned seed = -1);

    // load map from file, more info about file format in README
    // if creatures == true, all non g, w or t chars are considered creatures,
    // otherwise an error message is shown and program aborts
    Terrain (std::string terrain_source_file_path, bool creatures);

    // retunrns true if a block is sand (it's soil next to a water block)
    bool isSand (unsigned x, unsigned y) const;

    unsigned getWidth () const;
    unsigned getHeight () const;

    gnd::Item getTileType (unsigned x, unsigned y) const;

    // returns the creatures read from the file
    // this method should only be called when the class was generated with
    // the load map from file constructor
    const std::list<gnd::TerrainCreature> getCreatures () const;

    bool terrainReadFromFile () const;

    // returns true if the selected cell is free
    bool cellIsWalkable (unsigned x, unsigned y) const;

    // set position at x y as occupied by a creature
    // returns wether the creature could be placed there
    bool placeCreature (unsigned x, unsigned y);

    // move creature from old_p to new_p
    // returns wether the creature could be placed there
    bool moveCreature (gnd::Point old_p, gnd::Point new_p);

private:

    bool terrain_read_from_file;
    unsigned width_, height_;
    gnd::Map terrain;
    std::list<gnd::TerrainCreature> creature_list;  // creatures found when
                                                    // parsing terrain file
    std::vector<std::vector<bool>> free_space;
};

#endif