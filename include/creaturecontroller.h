#ifndef CREATURECONTROLLER_H_
#define CREATURECONTROLLER_H_

#include <list>
#include <vector>
#include "terrain.h"
#include "creature.h"


class CreatureController
{
public:

    CreatureController (Terrain *terrain_);

    // generate random position for creatures
    // Low complexity values cause the creatures to generate in big groups,
    // and greater values mean smaller different groups
    void generateCreatures (unsigned complexity);

    // only used when terrain was generated with the load terrain from file
    // constructor
    void loadCreaturesFromFile ();

    void getCreatureTypes (std::vector<char> &chars) const;

    // returns a vector with all creatures and its probabilities
    bool changeCreatureProb (char letter, uint new_prob);

    void getCreatures (std::list<std::pair<char, gnd::Point>> &creature_p) const;

    // returns true if there is at least one creature alive
    bool actualizeCreatures (unsigned long iteration);

    ~CreatureController ();

private:

    // parse a creature read from the file
    void parseCreature (char letter, gnd::Point p);

    Terrain *terrain;
    std::vector<Creature*> creature_types;
    std::vector<uint> creature_probabilities;

    // map is divided into nxn cells each one with a list of creatures that are
    // inside of that cell
    std::vector<std::vector<std::list<Creature*>>> cell;
    const uint CELL_SIZE = 10;
    unsigned max_creature_viewrange; // determines how many cells will be given
                                     // to a creature when actualizing
    unsigned n_creatures;
};

#endif