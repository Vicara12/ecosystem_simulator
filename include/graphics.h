#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <vector>
#include <map>
#include <iostream>

#include "terrain.h"


class Graphics
{
public:

    Graphics (sf::RenderWindow &window, const Terrain &terrain);

    void draw (const std::list<std::pair<char, gnd::Point>> &creatures);

    ~Graphics();

private:

    sf::RenderWindow &window_;
    const Terrain &terrain_;


    std::vector<std::vector<sf::RectangleShape*>> tile_grid;
    std::list<sf::Sprite*> tree_sprites;

    uint box_size;

    const uint default_box_size = 30;
    const uint max_window_height = 1000;
    const uint max_window_width  = 1750;
};

#endif