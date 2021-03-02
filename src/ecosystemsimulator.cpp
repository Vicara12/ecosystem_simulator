#include "ecosystemsimulator.h"

#include <iostream>
#include <vector>
#include <SFML/Window.hpp>
#include <unistd.h>

#include "graphics.h"



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
    
    creatures = new CreatureController(terrain);

    creatures->generateCreatures(map_creature_complexity);
}


void EcosystemSimulator::readTerrainFile ()
{
    terrain = new Terrain(map_file_path, terrain_file_has_creatures);

    creatures = new CreatureController(terrain);

    if (terrain_file_has_creatures)
    {   
        creatures->loadCreaturesFromFile();
    }
    else
    {
        creatures->generateCreatures(map_creature_complexity);
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

        std::list<std::pair<char, gnd::Point>> creature_p;

        creatures->getCreatures(creature_p);

        graphics.draw(creature_p);

        usleep(1e5);
    }
}


EcosystemSimulator::~EcosystemSimulator ()
{
    delete terrain;
}