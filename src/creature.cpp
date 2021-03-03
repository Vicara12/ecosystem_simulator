#include "creature.h"


Creature::Creature (char letter, uint probability, uint sight_range) :
    terrain_(NULL),
    dead(false),
    sight(sight_range),
    position(0,0),
    dummy_creature(true),
    LETTER(letter),
    type_(Type::Plant),
    prob(probability)
{
    //
}


Creature::Creature (Terrain &terrain,
                    Type type,
                    gnd::Point initial_pos,
                    char letter,
                    uint sight_range) :
    terrain_(&terrain),
    dead(false),
    sight(sight_range),
    position(initial_pos),
    dummy_creature(false),
    LETTER(letter),
    type_(type)
{
    //
}


bool Creature::isDead () const
{
    return dead;
}


struct gnd::Point Creature::getPos () const
{
    return position;
}


Creature::Type Creature::getType () const
{
    return type_;
}


uint Creature::getProbability () const
{
    if (not dummy_creature)
        throw new NotDummyCreature();
    
    return prob;
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


unsigned Creature::getSight () const
{
    return sight;
}


const char* Creature::DummyCreature::what () const throw ()
{
    return "attempted to actualize a dummy creature";
}


const char* Creature::NotDummyCreature::what () const throw ()
{
    return "attempted to get probability of non dummy creature";
}


bool Creature::changePos (gnd::Point new_p)
{
    if (not terrain_->cellIsWalkable(new_p.x, new_p.y))
        return false;
    
    terrain_->moveCreature(position, new_p);
    position = new_p;
    return true;
}


Creature* Creature::reproduced ()
{
    Creature *son_copy = son;
    son = NULL;
    
    return son_copy;
}