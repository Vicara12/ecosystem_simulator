#ifndef CREATURE_H_
#define CREATURE_H_

#include "terrain.h"



class Creature
{
public:

    enum Type {Plant, Bunny, Fox};

    Creature (const gnd::Map &terrain);

    virtual void actualize () = 0;

    struct gnd::Point getPos () const;

    Type getType () const;


private:

    gnd::Point position;
    const gnd::Map &terrain;
};


#endif