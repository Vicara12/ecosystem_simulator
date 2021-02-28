#ifndef BUNNY_H_
#define BUNNY_H_

#include "creature.h"


class Bunny : public Creature
{
public:

    // dummy creature constructor (for more info check creature class)
    Bunny ();

    // get a new Plant
    Creature* getNewCreature (const Terrain &terrain,
                              gnd::Point initial_pos);

    void actualize ();

    ~Bunny ();

private:

    Bunny (const Terrain &terrain, gnd::Point initial_pos);
};

#endif