#include "creaturecontroller.h"

#include <iostream>
#include "noisemap.h"
#include "plant.h"
#include "bunny.h"
#include "fox.h"


CreatureController::CreatureController (Terrain *terrain_) :
    terrain(terrain_)
{
    creature_types =
    // ADD NEW CREATURES HERE
    {
        new Plant(),
        new Bunny(),
        new Fox()
    };
}


void CreatureController::getCreatureTypes (std::vector<char> &chars) const
{
    chars.resize(creature_types.size());

    for (uint i = 0; i < creature_types.size(); i++)
        chars[i] = creature_types[i]->getLetter();
}


bool CreatureController::changeCreatureProb (char letter, uint new_prob)
{
    uint pos = 0;

    while (pos < creature_types.size() and
           creature_types[pos]->getLetter() != letter)
        pos++;
    
    if (pos == creature_types.size())
        return false;
    
    creature_probabilities[pos] = new_prob;
    return true;
}


void CreatureController::getCreatures (std::list<std::pair<char, gnd::Point>> &creature_p) const
{
    creature_p.clear();

    for (auto creature : creatures)
    {
        creature_p.push_back(std::make_pair(creature->getLetter(),
                                            creature->getPos()));
    }
}


void CreatureController::loadCreaturesFromFile ()
{
    std::list<gnd::TerrainCreature> file_creatures = terrain->getCreatures();

    for (auto tc : file_creatures)
        parseCreature(tc.character, tc.pos);
}


void CreatureController::generateCreatures (unsigned complexity)
{
    nm::NoiseMap perlin_noise_generator;
    nm::Map noise_map;

    uint height_ = terrain->getHeight();
    uint width_  = terrain->getWidth();

    unsigned resolution = (height_ > width_ ? height_ : width_) / complexity;
    unsigned noise_map_width = (width_/resolution +
                                (width_%resolution != 0));
    unsigned noise_map_height = (height_/resolution +
                                (height_%resolution != 0));
    
    std::vector<std::vector<bool>> cell_occupied(width_,
                                                 std::vector<bool>(height_,false));

    for (uint n = 0; n < creature_types.size(); n++)
    {
        // generate a new noise map for each creature type (because if just
        // one were used, all animals would be near each other)
        perlin_noise_generator.generateMap(noise_map_width, noise_map_height,
                                           resolution, noise_map);
        
        for (int i = 0; i < width_; i++)
        {
            for (int j = 0; j < height_; j++)
            {
                unsigned prob = creature_types[n]->getProbability();

                if (terrain->cellIsWalkable(i, j) and
                    nm::randomBool(noise_map[i][j]*(prob/100.f)))
                {
                    gnd::Point p(i,j);
                    creatures.push_back(creature_types[n]->getNewCreature(*terrain, p));                }
            }
        }
    }
}


void CreatureController::parseCreature (char letter, gnd::Point p)
{
    unsigned pos = 0;

    // search for a criature with the same letter than the one found
    while (pos < creature_types.size() and
            letter != creature_types[pos]->getLetter())
    {
        pos++;
    }

    // if creature not found
    if (pos == creature_types.size())
    {
        std::cerr << "ERROR: could not parse creature " << letter
                  << " at position " << p.x << "x " << p.y << "y. Continuing.\n";
    }

    // if found get a new creature of that type and add it to the list
    else
    {
        creatures.push_back(creature_types[pos]->getNewCreature(*terrain, p));
    }
}


CreatureController::~CreatureController ()
{
    for (auto ptr : creatures)
        delete ptr;

    for (auto ptr : creature_types)
        delete ptr;
}