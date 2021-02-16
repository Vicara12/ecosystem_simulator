#include "ecosystemsimulator.h"

#include <iostream>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
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
        map_complexity (3),
        map_qty_of_terrain (60),
        map_qty_of_trees (40),
        seed(-1),
        map_creature_complexity(2),
        map_file_path("")
{
    available_creatures =
    // ADD NEW CREATURES HERE
    {
        {'p', 50},
        {'b', 30},
        {'f', 10}
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
    terrain->generateCreatures(available_creatures, map_creature_complexity);

    std::list<gnd::TerrainCreature> file_creatures = terrain->getCreatures();
        
    parseCreatures(file_creatures);
}


void EcosystemSimulator::parseCreatures (const std::list<gnd::TerrainCreature> &unparsed_creatures)
{
    for (gnd::TerrainCreature creature : unparsed_creatures)
    {
        switch (creature.character)
        {
            case 'p':
            {
                creatures.push_back(new Plant(*terrain, creature.pos));
                break;
            }
            case 'b':
            {
                creatures.push_back(new Bunny(*terrain, creature.pos));
                break;
            }
            case 'f':
            {
                creatures.push_back(new Fox(*terrain, creature.pos));
                break;
            }
            // ADD NEW CREATURES HERE
            default:
            {
                std::cerr << "ERROR: could not parse creature "
                    << creature.character << " at position " << creature.pos.x
                    << "x " << creature.pos.y << "y. Continuing.\n";
            }
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