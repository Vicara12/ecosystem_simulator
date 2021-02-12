#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <vector>
#include <iostream>

#include "terrain.h"
#include "creature.h"


// properties for the graphics (size, colors, etc)
namespace gratr
{
    const sf::Color GRASS_COLOR  = sf::Color( 45, 202, 111);
    const sf::Color SAND_COLOR   = sf::Color(182, 201,  50);
    const sf::Color WATER_COLOR  = sf::Color(166, 215, 216);
    const sf::Color TREE_COLOR   = sf::Color( 85, 126,  57);
    const sf::Color BORDER_COLOR = sf::Color(  0,   0,   0);

    const unsigned BOX_SIZE = 40;   // box size in pixels
    const unsigned LATERAL_MARGIN = 10; // space between the border and the map

    const std::string TREE_SPRITE_PATH = "./textures/tree.png";
    const float TREE_SPRITE_SCALE = 0.025;
}


class Graphics
{
public:

    Graphics (sf::RenderWindow &window, const Terrain &terrain);

    void draw (const std::list<Creature*> &creatures);

private:

    sf::RenderWindow &window_;
    const Terrain &terrain_;
    bool tree_texture_loaded;

    std::vector<std::vector<sf::RectangleShape*>> tile_grid;
    sf::Texture tree_texture;
    std::list<sf::Sprite*> tree_sprites;
};

#endif