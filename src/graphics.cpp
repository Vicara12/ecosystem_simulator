#include "graphics.h"


Graphics::Graphics (sf::RenderWindow &window, const Terrain &terrain) :
        window_(window),
        terrain_(terrain),
        tile_grid(terrain_.getWidth(),
                  std::vector<sf::RectangleShape*>(terrain_.getHeight()))
{
    // init window
    window_.create(sf::VideoMode(2*gratr::LATERAL_MARGIN +
                                        terrain_.getWidth()*gratr::BOX_SIZE,
                                 2*gratr::LATERAL_MARGIN +
                                        terrain_.getHeight()*gratr::BOX_SIZE),
                   "Ecosystem simulator");

    window.setVerticalSyncEnabled(true);

    // load tree texture
    tree_texture_loaded = true;

    if (not tree_texture.loadFromFile(gratr::TREE_SPRITE_PATH))
    {
        tree_texture_loaded = false;

        std::cerr << "ERROR: failed to load tree texture. Continuing.\n";
    }
    else
    {
        tree_texture.setSmooth(true);
    }

    // init ground tiles
    for (int i = 0; i < terrain_.getWidth(); i++)
    {
        for (int j = 0; j < terrain_.getHeight(); j++)
        {
            tile_grid[i][j] = new sf::RectangleShape(
                                                 sf::Vector2f(gratr::BOX_SIZE,
                                                              gratr::BOX_SIZE));

            // set color
            if (terrain_.getTileType(i, j) == gnd::Item::Water)
                tile_grid[i][j]->setFillColor(gratr::WATER_COLOR);
            else if (terrain_.isSand(i, j))
                tile_grid[i][j]->setFillColor(gratr::SAND_COLOR);
            else
                tile_grid[i][j]->setFillColor(gratr::GRASS_COLOR);
            
            // set position
            tile_grid[i][j]->setPosition(sf::Vector2f(
                            float(gratr::LATERAL_MARGIN + i*gratr::BOX_SIZE),
                            float(gratr::LATERAL_MARGIN + j*gratr::BOX_SIZE)));
            
            // handle trees
            if (terrain_.getTileType(i, j) == gnd::Item::Tree)
            {
                // if texture has been loaded, fill tree list
                if (tree_texture_loaded)
                {
                    // generate, shape and locate tree stprite
                    tree_sprites.push_back(new sf::Sprite());
                    tree_sprites.back()->setTexture(tree_texture);
                    tree_sprites.back()->setPosition(sf::Vector2f(
                            float(gratr::LATERAL_MARGIN + i*gratr::BOX_SIZE),
                            float(gratr::LATERAL_MARGIN + j*gratr::BOX_SIZE)));
                    tree_sprites.back()->setScale(sf::Vector2f(
                                                    gratr::TREE_SPRITE_SCALE,
                                                    gratr::TREE_SPRITE_SCALE));
                }
                // else just change cell color to tree color
                else
                    tile_grid[i][j]->setFillColor(gratr::TREE_COLOR);
            }
        }
    }
}


void Graphics::draw (const std::list<Creature*> &creatures)
{
    window_.clear(gratr::BORDER_COLOR);

    // draw tiles
    for (int i = 0; i < terrain_.getWidth(); i++)
        for (int j = 0; j < terrain_.getHeight(); j++)
            window_.draw(*tile_grid[i][j]);
    
    // draw trees
    if (tree_texture_loaded)
        for (auto tree : tree_sprites)
            window_.draw(*tree);

    // draw creatures
    // ~

    window_.display();
}