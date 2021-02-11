#include "graphics.h"


Graphics::Graphics (sf::RenderWindow &window, const Terrain &terrain) :
        window_(window),
        terrain_(terrain),
        tile_grid(terrain.getHeight(),
                  std::vector<sf::RectangleShape>(terrain.getWidth()))
{
    // load tree texture
    tree_texture_loaded = true;

    if (not tree_texture.loadFromFile(gratr::TREE_SPRITE_PATH))
    {
        tree_texture_loaded = false;

        std::cerr << "ERROR: failed to load tree texture. Continuing.\n";
    }
    else
    {
        tree_sprite.setTexture(tree_texture);
    }


    // init ground tiles
    for (int i = 0; i < terrain_.getWidth(); i++)
    {
        for (int j = 0; j < terrain_.getHeight(); j++)
        {
            // set color
            if (terrain_.getTileType(i, j) == gnd::Item::Water)
                tile_grid[i][j].setFillColor(gratr::WATER_COLOR);
            else if (terrain_.isSand(i, j))
                tile_grid[i][j].setFillColor(gratr::SAND_COLOR);
            else
                tile_grid[i][j].setFillColor(gratr::GRASS_COLOR);

            // set size and position
            tile_grid[i][j].setSize(sf::Vector2f(float(gratr::BOX_SIZE),
                                                 float(gratr::BOX_SIZE)));
            tile_grid[i][j].setPosition(sf::Vector2f(
                            float(gratr::LATERAL_MARGIN + i*gratr::BOX_SIZE),
                            float(gratr::LATERAL_MARGIN + j*gratr::BOX_SIZE)));
            
            // if cell is a tree and sprite could not be load it, set color
            if (terrain_.getTileType(i, j) == gnd::Item::Tree and
                not tree_texture_loaded)
            {
                tile_grid[i][j].setFillColor(gratr::TREE_COLOR);
            }
        }
    }
}


void Graphics::draw (const std::list<Creature> &creatures)
{
    window_.clear(gratr::BORDER_COLOR);

    // draw tiles
    for (int i = 0; i < terrain_.getWidth(); i++)
        for (int j = 0; j < terrain_.getHeight(); j++)
            window_.draw(tile_grid[i][j]);

    // draw creatures
    // ~

    window_.display();
}