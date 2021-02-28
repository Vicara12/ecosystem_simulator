#include "bunny.h"


Bunny::Bunny () :
    Creature('b')
{
    //
}


Creature* Bunny::getNewCreature (const Terrain &terrain,
                                 gnd::Point initial_pos)
{
    return new Bunny(terrain, initial_pos);
}


Bunny::Bunny (const Terrain &terrain, gnd::Point initial_pos) :
        Creature(terrain, Creature::Type::Bunny, initial_pos, 'b')
{
    //
}


void Bunny::actualize ()
{
    checkDummy();
}


Bunny::~Bunny ()
{
    //
}