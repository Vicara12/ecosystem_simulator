#ifndef CREATURE_H_
#define CREATURE_H_

#include "terrain.h"


/*

To create a new creature build a new class, program the actualize method with
its behaviour, insert the new type in the enum, add it to the texture map in
the gratr namespace with its corresponding texture and finally add it to the
creature parser in the parseCreatures method and to the constructor available
creatures option with its probability in the methods of the
ecosystem simulator class (also, add more program parameters to handle them in
the parseArguments method of the same class).
ALL CREATURES MUST HAVE A DESTRUCTOR

*/

class Creature
{
public:

    enum Type {Plant, Bunny, Fox};


    Creature (const Terrain &terrain, Type type, gnd::Point initial_pos);

    virtual void actualize () = 0;

    struct gnd::Point getPos () const;

    Type getType () const;

    virtual ~Creature ();


protected:

    gnd::Point position;
    const Type type_;
    const Terrain &terrain_;
};


#endif