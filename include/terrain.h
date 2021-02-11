#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <list>
#include <vector>
#include "ground.h"


class Terrain
{
public:

    // generate random map with the specified dimensions
    Terrain (unsigned width, unsigned height);
    // load map from file, more info about file format in README
    // if creatures == true, all non g, w or t chars are considered creatures,
    // otherwise an error message is shown and program aborts
    Terrain (std::string terrain_source_file_path, bool creatures);

    // retunrns true if a block is sand (it's soil next to a water block)
    bool isSand (unsigned x, unsigned y) const;

    unsigned getWidth () const;
    unsigned getHeight () const;

    gnd::Item getTileType (unsigned x, unsigned y) const;

    // when parsing a terrain file, it stores all found creatures in a list
    // with the row and col it was found and the corresponding character
    const std::list<gnd::TerrainCreature> getFileCreatures () const;

private:

    unsigned width_, height_;
    gnd::Map terrain;
    std::list<gnd::TerrainCreature> file_creatures; // creatures found when
                                                    // parsing terrain file
};

#endif