#ifndef BUNNY_H_
#define BUNNY_H_

#include "creature.h"


class Bunny : public Creature
{
public:

    Bunny (const Terrain &terrain, gnd::Point initial_pos);

    void actualize ();

private:
};

#endif