#ifndef FOX_H_
#define FOX_H_

#include "creature.h"


class Fox : public Creature
{
public:

    Fox (const Terrain &terrain, gnd::Point initial_pos);

    void actualize ();

    ~Fox ();

private:
};

#endif