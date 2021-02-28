#ifndef PLANT_H_
#define PLANT_H_

#include "creature.h"

class Plant : public Creature
{
public:

    // dummy creature constructor (for more info check creature class)
    Plant ();

    // get a new Plant
    Creature* getNewCreature (Terrain &terrain,
                              gnd::Point initial_pos);

    void actualize (unsigned long iteration);

    ~Plant ();

private:

    Plant (Terrain &terrain, gnd::Point initial_pos);
};

#endif