#ifndef PLANT_H_
#define PLANT_H_

#include "creature.h"

class Plant : public Creature
{
public:

    Plant (const Terrain &terrain, gnd::Point initial_pos);

    void actualize ();

    ~Plant ();

private:
};

#endif