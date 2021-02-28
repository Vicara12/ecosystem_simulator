#include "graphics.h"


static unsigned min (uint a, uint b)
{
    return (a < b ? a : b);
}


// properties for the graphics (size, colors, etc)
namespace gratr
{
    const sf::Color GRASS_COLOR  = sf::Color( 45, 202, 111);
    const sf::Color SAND_COLOR   = sf::Color(182, 201,  50);
    const sf::Color WATER_COLOR  = sf::Color(123, 219, 221);
    const sf::Color BORDER_COLOR = sf::Color(  0,   0,   0);

    const unsigned LATERAL_MARGIN = 10; // space between the border and the map

    // struct used to hold info for each texture
    struct EntityTexture {
        sf::Texture texture;
        bool loaded;
        unsigned width, height; // in pixels
    
        EntityTexture() {};

        // empty crop vector means no cropping
        EntityTexture (std::string path, uint width, uint height);
    };

    // declaration of the tree texture
    const struct EntityTexture TREE_TEXTURE("./res/tree.png", 1600, 1600);

    // map with the textures of all creatures
    // ADD TEXTURES FOR NEW CREATURES HERE
    std::map<Creature::Type, EntityTexture> CREATURE_TEXTURES =
    {
        {Creature::Type::Plant, EntityTexture("./res/plant.png", 360, 360)},
        {Creature::Type::Bunny, EntityTexture("./res/bunny.png", 474, 474)},
        {Creature::Type::Fox,   EntityTexture("./res/fox.png",   474, 474)}
    };
}



gratr::EntityTexture::EntityTexture (std::string path, uint width, uint height)
{
    this->width = width;
    this->height = height;
    loaded = texture.loadFromFile(path, sf::IntRect(0, 0, width, height));

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
    box_size = default_box_size;

    if (box_size*terrain_.getWidth() > max_window_width or
        box_size*terrain_.getHeight() > max_window_height)
    {
        box_size = min(max_window_height/terrain_.getHeight(),
                       max_window_width/terrain_.getWidth());
    }

    // init window
    window_.create(sf::VideoMode(2*gratr::LATERAL_MARGIN +
                                        terrain_.getWidth()*box_size,
                                 2*gratr::LATERAL_MARGIN +
                                        terrain_.getHeight()*box_size),
                   "Ecosystem simulator");

    window.setVerticalSyncEnabled(true);

    // init ground tiles
    for (int i = 0; i < terrain_.getWidth(); i++)
    {
        for (int j = 0; j < terrain_.getHeight(); j++)
        {
            tile_grid[i][j] = new sf::RectangleShape(
                                                 sf::Vector2f(box_size,
                                                              box_size));

            // set color
            if (terrain_.getTileType(i, j) == gnd::Item::Water)
                tile_grid[i][j]->setFillColor(gratr::WATER_COLOR);
            else if (terrain_.isSand(i, j))
                tile_grid[i][j]->setFillColor(gratr::SAND_COLOR);
            else
                tile_grid[i][j]->setFillColor(gratr::GRASS_COLOR);
            
            // set position
            tile_grid[i][j]->setPosition(sf::Vector2f(
                            float(gratr::LATERAL_MARGIN + i*box_size),
                            float(gratr::LATERAL_MARGIN + j*box_size)));
            
            // handle trees
            if (terrain_.getTileType(i, j) == gnd::Item::Tree)
            {
                // if texture has been loaded, fill tree list
                if (gratr::TREE_TEXTURE.loaded)
                {
                    double scale_x = double(box_size)/gratr::TREE_TEXTURE.width;
                    double scale_y = double(box_size)/gratr::TREE_TEXTURE.height;

                    // generate, shape and locate tree stprite
                    tree_sprites.push_back(new sf::Sprite());
                    tree_sprites.back()->setTexture(gratr::TREE_TEXTURE.texture,
                                                    true);
                    tree_sprites.back()->setPosition(sf::Vector2f(
                            float(gratr::LATERAL_MARGIN + i*box_size),
                            float(gratr::LATERAL_MARGIN + j*box_size)));
                    tree_sprites.back()->setScale(sf::Vector2f( scale_x, scale_y));
                }
                // else just change cell color to green
                else
                    tile_grid[i][j]->setFillColor(sf::Color::Green);
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
    sf::RectangleShape auxiliar_shape(sf::Vector2f(box_size, box_size));

    // draw creatures
    for (auto creature : creatures)
    {
        const gratr::EntityTexture &et = gratr::CREATURE_TEXTURES[creature->getType()];

        if (et.loaded)
        {
            double scale_x = double(box_size)/et.width;
            double scale_y = double(box_size)/et.height;

            sprite.setTexture(et.texture, true);
        
            sprite.setPosition(sf::Vector2f(gratr::LATERAL_MARGIN +
                                                creature->getPos().x*box_size,
                                            gratr::LATERAL_MARGIN +
                                                creature->getPos().y*box_size));
            sprite.setScale(sf::Vector2f(scale_x, scale_y));

            window_.draw(sprite);
        }
        else
        {
            auxiliar_shape.setFillColor(sf::Color::Black);
            auxiliar_shape.setPosition(sf::Vector2f(gratr::LATERAL_MARGIN +
                                creature->getPos().x*box_size,
                                    gratr::LATERAL_MARGIN +
                                creature->getPos().y*box_size));
            
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