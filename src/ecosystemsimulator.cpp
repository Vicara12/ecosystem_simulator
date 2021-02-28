#include "ecosystemsimulator.h"

#include <iostream>
#include <vector>
#include <SFML/Window.hpp>
#include <unistd.h>

#include "graphics.h"
#include "plant.h"
#include "bunny.h"
#include "fox.h"



EcosystemSimulator::EcosystemSimulator (int argc, char** argv) :
        read_terrain_from_file (false),
        terrain_file_has_creatures (false),
        map_height (25),
        map_width (35),
        map_complexity (5),
        map_qty_of_terrain (50),
        map_qty_of_trees (40),
        seed(-1),
        map_creature_complexity(2),
        map_file_path("")
{
    creature_types =
    // ADD NEW CREATURES HERE
    {
        {new Plant(), 50},
        {new Bunny(), 30},
        {new Fox(), 10}
    };

    parseArguments(argc, argv);

    if (read_terrain_from_file)
        readTerrainFile();
    else
        generateTerrain();
}


void EcosystemSimulator::parseArguments (int argc, char** argv)
{
    //
}


void EcosystemSimulator::generateTerrain ()
{
    terrain = new Terrain(map_width, map_height, map_complexity,
                          map_qty_of_terrain, map_qty_of_trees, seed);

    generateCreatures();
}


void EcosystemSimulator::readTerrainFile ()
{
    terrain = new Terrain(map_file_path, terrain_file_has_creatures);

    if (terrain_file_has_creatures)
    {
        std::list<gnd::TerrainCreature> file_creatures = terrain->getCreatures();
        
        parseCreatures(file_creatures);
    }
    else
    {
        generateCreatures();
    }
}


void EcosystemSimulator::generateCreatures ()
{
    std::vector<std::pair<char, uint>> creature_chars(creature_types.size());

    for (int i = 0; i < creature_chars.size(); i++)
    {
        creature_chars[i].first = creature_types[i].first->getLetter();
        creature_chars[i].second = creature_types[i].second;
    }

    terrain->generateCreatures(creature_chars, map_creature_complexity);

    std::list<gnd::TerrainCreature> file_creatures = terrain->getCreatures();
        
    parseCreatures(file_creatures);
}


void EcosystemSimulator::parseCreatures (const std::list<gnd::TerrainCreature> &unparsed_creatures)
{
    for (gnd::TerrainCreature creature : unparsed_creatures)
    {
        unsigned pos = 0;

        // search for a criature with the same letter than the one found
        while (pos < creature_types.size() and
               creature.character != creature_types[pos].first->getLetter())
        {
            pos++;
        }

        // if creature not found
        if (pos == creature_types.size())
        {
            std::cerr << "ERROR: could not parse creature "
                << creature.character << " at position " << creature.pos.x
                << "x " << creature.pos.y << "y. Continuing.\n";
        }
        // if found get a new creature of that type and add it to the list
        else
        {
            creatures.push_back(creature_types[pos].first->getNewCreature(*terrain, creature.pos));
        }
    }
}


void EcosystemSimulator::run ()
{
    sf::RenderWindow window;

    Graphics graphics(window, *terrain);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        graphics.draw(creatures);

        usleep(1e5);
    }
}


EcosystemSimulator::~EcosystemSimulator ()
{
    delete terrain;
    
    for (auto ptr : creatures)
        delete ptr;
}