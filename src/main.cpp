#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <list>
#include "terrain.h"
#include "graphics.h"
#include "plant.h"
using namespace std;

int main ()
{
    sf::RenderWindow window;

    Terrain terrain("./test/terrain", false);

    Graphics graphics(window, terrain);

    std::list<Creature*> creatures;

    creatures.push_back(new Plant(terrain, gnd::Point(1,1)));

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        graphics.draw(creatures);

        usleep(1e5);
    }
}