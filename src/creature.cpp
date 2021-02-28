#include "creature.h"


Creature::Creature (char letter) :
    position(0,0),
    dummy_creature(true),
    LETTER(letter),
    type_(Type::Plant),
    terrain_(NULL)
{
    //
}


Creature::Creature (const Terrain &terrain,
                    Type type,
                    gnd::Point initial_pos,
                    char letter) :
    position(initial_pos),
    dummy_creature(false),
    LETTER(letter),
    type_(type),
    terrain_(&terrain)
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


Creature::~Creature ()
{
    //
}


void Creature::checkDummy () const
{
    if (dummy_creature)
        throw new DummyCreature();
}


char Creature::getLetter () const
{
     return LETTER;
}


const char* Creature::DummyCreature::what () const throw ()
{
    return "attempted to actualize a dummy creature";
}