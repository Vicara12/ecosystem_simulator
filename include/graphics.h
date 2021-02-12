#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <vector>
#include <map>
#include <iostream>

#include "terrain.h"
#include "creature.h"


class Graphics
{
public:

    Graphics (sf::RenderWindow &window, const Terrain &terrain);

    void draw (const std::list<Creature*> &creatures);

    ~Graphics();

private:

    sf::RenderWindow &window_;
    const Terrain &terrain_;

    std::vector<std::vector<sf::RectangleShape*>> tile_grid;
    std::list<sf::Sprite*> tree_sprites;
};

#endif