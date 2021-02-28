#include "fox.h"


Fox::Fox () :
    Creature('f')
{
    //
}


Creature* Fox::getNewCreature (const Terrain &terrain,
                               gnd::Point initial_pos)
{
    return new Fox(terrain, initial_pos);
}


Fox::Fox (const Terrain &terrain, gnd::Point initial_pos) :
        Creature(terrain, Creature::Type::Fox, initial_pos, 'f')
{
    //
}


void Fox::actualize ()
{
    checkDummy();
}

Fox::~Fox ()
{
    //
}