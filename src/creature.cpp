#include "creature.h"


Creature::Creature (const Terrain &terrain, Type type, gnd::Point initial_pos) :
    position(initial_pos),
    type_(type),
    terrain_(terrain)
{
    //
}


struct gnd::Point Creature::getPos () const
{
    return position;
}


Creature::Type Creature::getType () const
{
    return type_;
}