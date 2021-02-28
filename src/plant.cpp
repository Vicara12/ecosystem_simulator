#include "plant.h"



Plant::Plant () :
    Creature('p')
{
    //
}


Creature* Plant::getNewCreature (Terrain &terrain,
                                 gnd::Point initial_pos)
{
    return new Plant(terrain, initial_pos);
}


Plant::Plant (Terrain &terrain, gnd::Point initial_pos) :
        Creature(terrain, Creature::Type::Plant, initial_pos, 'p')
{
    //
}


void Plant::actualize (unsigned long iteration)
{
    checkDummy();
}


Plant::~Plant ()
{
    //
}