#ifndef ECOSYSTEM_SIMULATOR_H_
#define ECOSYSTEM_SIMULATOR_H_

#include <list>
#include <string>
#include "terrain.h"
#include "creaturecontroller.h"
#include "ground.h"


class EcosystemSimulator
{
public:

    EcosystemSimulator (int argc, char** arg);

    void run ();

    ~EcosystemSimulator();

private:

    // parse main program arguments
    void parseArguments (int argc, char** argv);

    // handle terrain creation
    void generateTerrain ();
    void readTerrainFile ();
    void generateCreatures ();
    void parseCreatures (const std::list<gnd::TerrainCreature> &unparsed_creatures);


    Terrain* terrain;
    CreatureController *creatures;

    // terrain generation variables
    bool read_terrain_from_file;
    bool terrain_file_has_creatures;

    unsigned map_height, map_width, map_complexity;
    unsigned map_qty_of_terrain, map_qty_of_trees, seed;
    unsigned map_creature_complexity;

    std::string map_file_path;
};

#endif