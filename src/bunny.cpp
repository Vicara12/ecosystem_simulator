#include "bunny.h"


Bunny::Bunny () :
    Creature('b', 30) // default char and probability defined here
{
    //
}


Creature* Bunny::getNewCreature (Terrain &terrain,
                                 gnd::Point initial_pos)
{
    return new Bunny(terrain, initial_pos);
}


Bunny::Bunny (Terrain &terrain, gnd::Point initial_pos) :
        Creature(terrain, Creature::Type::Bunny, initial_pos, 'b')
{
    //
}


void Bunny::actualize (unsigned long iteration)
{
    checkDummy();
}


Bunny::~Bunny ()
{
    //
}