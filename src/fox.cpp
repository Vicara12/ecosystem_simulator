#include "fox.h"


Fox::Fox () :
    Creature('f', 10) // default char and probability defined here
{
    //
}


Creature* Fox::getNewCreature (Terrain &terrain,
                               gnd::Point initial_pos)
{
    return new Fox(terrain, initial_pos);
}


Fox::Fox (Terrain &terrain, gnd::Point initial_pos) :
        Creature(terrain, Creature::Type::Fox, initial_pos, 'f')
{
    //
}


void Fox::actualize (unsigned long iteration)
{
    checkDummy();
}

Fox::~Fox ()
{
    //
}