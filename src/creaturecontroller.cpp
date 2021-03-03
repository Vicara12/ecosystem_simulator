#include "creaturecontroller.h"

#include <iostream>
#include "noisemap.h"
#include "plant.h"
#include "bunny.h"
#include "fox.h"


CreatureController::CreatureController (Terrain *terrain_) :
    terrain(terrain_),
    max_creature_viewrange(0),
    n_creatures(0)
{
    creature_types =
    // ADD NEW CREATURES HERE
    {
        new Plant(),
        new Bunny(),
        new Fox()
    };

    // set the view range with the creature with better sight
    for (auto creature : creature_types)
    {
        uint creature_range = creature->getSight();

        if (max_creature_viewrange < creature_range)
            max_creature_viewrange = creature_range;
    }

    // setup cells
    uint width  = terrain->getWidth();
    uint height = terrain->getHeight();
    // size of the vector (number of cells in each direction)
    uint map_width  = width/CELL_SIZE + (width%CELL_SIZE);
    uint map_height = height/CELL_SIZE + (height%CELL_SIZE);

    cell = std::vector<std::vector<std::list<Creature*>>>(map_width,
                                    std::vector<std::list<Creature*>>(map_height));
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

    for (int i = 0; i < cell.size(); i++)
    {
        for (int j = 0; j < cell[0].size(); j++)
        {
            for (auto creature : cell[i][j])
                creature_p.push_back(std::make_pair(creature->getLetter(),
                                                    creature->getPos()));
        }
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
                    Creature *new_c = creature_types[n]->getNewCreature(*terrain, p);

                    // add to creatures cell structure
                    cell[p.x/CELL_SIZE][p.y/CELL_SIZE].push_back(new_c);
                }
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
        Creature *new_c = creature_types[pos]->getNewCreature(*terrain, p);

        // add to creatures cell structure
        cell[p.x/CELL_SIZE][p.y/CELL_SIZE].push_back(new_c);
    }
}


CreatureController::~CreatureController ()
{
    // delete all normal creatures
    for (int i = 0; i < cell.size(); i++)
        for (int j = 0; j < cell[0].size(); j++)
            for (auto ptr : cell[i][j])
                delete ptr;

    // delete all dummy creatures
    for (auto ptr : creature_types)
        delete ptr;
}


bool CreatureController::actualizeCreatures (unsigned long iteration)
{
    // call actualize for each creature
    for (uint i = 0; i < cell.size(); i++)
    {
        for (uint j = 0; j < cell[0].size(); j++)
        {
            // build the list of nearby creatures
            std::list<Creature*> nearby_c;

            uint cell_radious = max_creature_viewrange/CELL_SIZE;

            // get all the creatures in the surroungind cell_radious cells
            for (int x = i-cell_radious; x <= i+cell_radious; x++)
            {
                if (x < 0 or x >= cell.size())
                    continue;
                
                for (int y = j-cell_radious; y <= j+cell_radious; y++)
                {
                    if (y < 0 or y > cell[0].size())
                        continue;
                    
                    for (auto creature : cell[x][y])
                        nearby_c.push_back(creature);
                }
            }

            // actualize all cratures in the cell
            for (auto creature : cell[i][j])
            {
                creature->actualize(iteration, nearby_c);

                Creature *new_c = creature->reproduced();

                if (new_c != NULL)
                {
                    cell[i][j].push_back(new_c);
                    nearby_c.push_back(new_c);
                }
                
                if (creature->isDead())
                {
                    delete creature;
                    nearby_c.remove(creature);
                    cell[i][j].remove(creature);
                }

                // it feels kinda creepy to check if the creature diead and
                // reproduced in the same iteration...
            }
        }
    }
}