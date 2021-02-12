#ifndef CREATURE_H_
#define CREATURE_H_

#include "terrain.h"


/*

To create a new creature build a new class, program the actualize method with
its behaviour, init all static parameters, insert the new type in the enum and
add it to the texture map in the gratr namespace

*/

class Creature
{
public:

    enum Type {Plant, Bunny, Fox};


    Creature (const Terrain &terrain, Type type, gnd::Point initial_pos);

    virtual void actualize () = 0;

    struct gnd::Point getPos () const;

    Type getType () const;


protected:

    gnd::Point position;
    const Type type_;
    const Terrain &terrain_;
};


#endif