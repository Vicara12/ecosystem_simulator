#ifndef FOX_H_
#define FOX_H_

#include "creature.h"


class Fox : public Creature
{
public:

    // dummy creature constructor (for more info check creature class)
    Fox ();
    
    // get a new Plant
    Creature* getNewCreature (Terrain &terrain,
                              gnd::Point initial_pos);

    void actualize (unsigned long iteration);

    ~Fox ();

private:

    Fox (Terrain &terrain, gnd::Point initial_pos);
};

#endif