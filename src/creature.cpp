#include "creature.h"


Creature::Creature (char letter) :
    terrain_(NULL),
    position(0,0),
    dummy_creature(true),
    LETTER(letter),
    type_(Type::Plant)
{
    //
}


Creature::Creature (Terrain &terrain,
                    Type type,
                    gnd::Point initial_pos,
                    char letter) :
    terrain_(&terrain),
    position(initial_pos),
    dummy_creature(false),
    LETTER(letter),
    type_(type)
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


bool Creature::changePos (gnd::Point new_p)
{
    if (not terrain_->cellIsWalkable(new_p.x, new_p.y))
        return false;
    
    terrain_->moveCreature(position, new_p);
    position = new_p;
    return true;
}