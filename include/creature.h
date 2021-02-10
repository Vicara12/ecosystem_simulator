#ifndef CREATURE_H_
#define CREATURE_H_

#include "ground.h"
#include "creature.h"



class Creature
{
public:

    enum Type {Bunny, Fox};

    Creature (const gnd::Map &terrain);

    virtual void actualize () = 0;

    void getPos (unsigned &x, unsigned &y);


private:

    gnd::Point position;
    const gnd::Map &terrain;
};


#endif