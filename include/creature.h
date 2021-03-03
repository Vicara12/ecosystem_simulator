#ifndef CREATURE_H_
#define CREATURE_H_

#include "terrain.h"
#include <exception>


/*

To create a new creature build a new class, program an ampty public constructor
for dummy creatures and a private for real ones, the actualize method with
its behaviour (also this method must call the checkDummy method), insert the
new type in the enum, add it to the texture map in the gratr namespace with
its corresponding texture and finally add it to the main class constructor
(ecosystem emulator) with its default probability.

ALL CREATURES MUST HAVE A DESTRUCTOR

*/

class Creature
{
public:

    // exceptions of the class
    struct DummyCreature;
    struct NotDummyCreature;


    enum Type {Plant, Bunny, Fox};

    // The constructur is only used to declare that a specific kind of creature
    // exists in the ecosystem simulator class, to get a new instance of a
    // creature the getNewCreatureMethod should be used instead
    Creature (char letter, uint probability, uint sight_range);

    // If this method gets called from a dummy creature, an exception is raised
    // It goes through an iteration of the creature's logic
    virtual void actualize (unsigned long iteration,
                            const std::list<Creature*> &nearby_c) = 0;

    bool isDead () const;

    // returns pointer to new creature if it reproduced or NULL otherwise
    Creature* reproduced ();

    // get a real creature
    virtual Creature* getNewCreature (Terrain &terrain,
                                      gnd::Point initial_pos) = 0;

    struct gnd::Point getPos () const;

    Type getType () const;

    char getLetter () const;

    unsigned getSight () const;

    // if 
    uint getProbability () const;

    virtual ~Creature ();


protected:

    Creature (Terrain &terrain,
              Type type,
              gnd::Point initial_pos,
              char letter,
              uint sight_range);
    
    bool changePos (gnd::Point new_p);

    // if creature is dummy, raises an exception
    void checkDummy () const;


    Terrain *terrain_;
    bool dead;
    uint sight;
    Creature *son; // when creature reproduces, the son is held here until
                   // the reproduced method gets called


private:

  
  gnd::Point position;
  bool dummy_creature; // true if the creature was created with the normal
                         // constructor instead of the getNewCreatureClass
  const char LETTER;
  const Type type_;
  uint prob; // indicates the density of creature generation of a given species
  
};


// method should not be called in dummy creature
struct Creature::DummyCreature : public std::exception
{
  const char* what () const throw ();
};


// method should not be called in non dummy creature
struct Creature::NotDummyCreature : public std::exception
{
  const char* what () const throw ();
};



#endif