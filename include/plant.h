#ifndef PLANT_H_
#define PLANT_H_

#include "creature.h"

class Plant : public Creature
{
public:

    // dummy creature constructor (for more info check creature class)
    Plant ();

    // get a new Plant
    Creature* getNewCreature (const Terrain &terrain,
                              gnd::Point initial_pos);

    void actualize ();

    ~Plant ();

private:

    Plant (const Terrain &terrain, gnd::Point initial_pos);
};

#endif