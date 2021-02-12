#include "graphics.h"



// properties for the graphics (size, colors, etc)
namespace gratr
{
    const sf::Color GRASS_COLOR  = sf::Color( 45, 202, 111);
    const sf::Color SAND_COLOR   = sf::Color(182, 201,  50);
    const sf::Color WATER_COLOR  = sf::Color(123, 219, 221);
    const sf::Color BORDER_COLOR = sf::Color(  0,   0,   0);

    const unsigned BOX_SIZE = 40;   // box size in pixels
    const unsigned LATERAL_MARGIN = 10; // space between the border and the map

    // struct used to hold info for each texture
    struct EntityTexture {
        sf::Texture texture;
        bool loaded;
        float scale;
        sf::Color alt_color; // color to place in case the texture doesn't laod
    
        EntityTexture() {};

        // empty crop vector means no cropping
        EntityTexture (std::string path,
                       float scale_,
                       sf::Color alt,
                       std::vector<unsigned> crop = std::vector<unsigned>());
    };

    // declaration of the tree texture
    const struct EntityTexture TREE_TEXTURE("./textures/tree.png", 0.025,
                                            sf::Color(85, 126, 57));

    // map with the textures of all creatures
    std::map<Creature::Type, EntityTexture> CREATURE_TEXTURES =
    {
        {Creature::Type::Plant, EntityTexture("./textures/plant.png", 0.12,
                                              sf::Color(85, 126, 57))},
        {Creature::Type::Bunny, EntityTexture("./textures/bunny.png", 0.08,
                                              sf::Color(85, 126, 57))},
        {Creature::Type::Fox,   EntityTexture("./textures/fox.png", 0.08,
                                              sf::Color(85, 126, 57))}
    };
}



gratr::EntityTexture::EntityTexture (std::string path,
                                     float scale_,
                                     sf::Color alt,
                                     std::vector<unsigned> crop)
{
    scale  = scale_;
    alt_color = alt;

    if (crop.empty())
        loaded = texture.loadFromFile(path);
    else
        loaded = texture.loadFromFile(path, sf::IntRect(crop[0], crop[1],
                                                        crop[2], crop[3]));

    if (not loaded)
    {
        std::cerr << "ERROR: failed to load texture " << path
                  <<" . Continuing.\n";
    }
    else
    {
        texture.setSmooth(true);
    }
}


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
                if (gratr::TREE_TEXTURE.loaded)
                {
                    // generate, shape and locate tree stprite
                    tree_sprites.push_back(new sf::Sprite());
                    tree_sprites.back()->setTexture(gratr::TREE_TEXTURE.texture);
                    tree_sprites.back()->setPosition(sf::Vector2f(
                            float(gratr::LATERAL_MARGIN + i*gratr::BOX_SIZE),
                            float(gratr::LATERAL_MARGIN + j*gratr::BOX_SIZE)));
                    tree_sprites.back()->setScale(sf::Vector2f(
                                                    gratr::TREE_TEXTURE.scale,
                                                    gratr::TREE_TEXTURE.scale));
                }
                // else just change cell color to tree color
                else
                    tile_grid[i][j]->setFillColor(gratr::TREE_TEXTURE.alt_color);
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
    if (gratr::TREE_TEXTURE.loaded)
        for (auto tree : tree_sprites)
            window_.draw(*tree);
    
    sf::Sprite sprite;
    // this rectangle is used when the texture could not be loaded
    sf::RectangleShape auxiliar_shape(sf::Vector2f(gratr::BOX_SIZE,
                                                   gratr::BOX_SIZE));

    // draw creatures
    for (auto creature : creatures)
    {
        const gratr::EntityTexture &et = gratr::CREATURE_TEXTURES[creature->getType()];

        if (et.loaded)
        {
            sprite.setTexture(et.texture);
        
            sprite.setPosition(sf::Vector2f(gratr::LATERAL_MARGIN +
                                            creature->getPos().x*gratr::BOX_SIZE,
                                            gratr::LATERAL_MARGIN +
                                            creature->getPos().y*gratr::BOX_SIZE));
            sprite.setScale(sf::Vector2f(et.scale, et.scale));

            window_.draw(sprite);
        }
        else
        {
            auxiliar_shape.setFillColor(et.alt_color);
            auxiliar_shape.setPosition(sf::Vector2f(gratr::LATERAL_MARGIN +
                                creature->getPos().x*gratr::BOX_SIZE,
                                    gratr::LATERAL_MARGIN +
                                creature->getPos().y*gratr::BOX_SIZE));
            
            window_.draw(auxiliar_shape);
        }
    }

    window_.display();
}


Graphics::~Graphics ()
{
    for (int i = 0; i < terrain_.getWidth(); i++)
        for (int j = 0; j < terrain_.getHeight(); j++)
            delete tile_grid[i][j];
    
    for (auto tree_sprite : tree_sprites)
        delete tree_sprite;
}