#include "terrain.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "noisemap.h"


Terrain::Terrain (unsigned width,
                  unsigned height,
                  unsigned terrain_complexity,
                  unsigned qty_of_terrain,
                  unsigned qty_of_trees,
                  unsigned seed) :
        width_(width),
        height_(height),
        terrain(width_, std::vector<gnd::Item>(height, gnd::Item::Grass))
{
    if (seed == unsigned(-1))
        nm::NoiseMap::setRandomSeed();
    else
        nm::NoiseMap::setSeed(seed);

    
    nm::NoiseMap perlin_noise_generator;

    // the size of the noise map must be a multiple of the resolution
    // (terrain complexity), so we must select a value for the width equal
    // to the smaller multiple of resolution that is greater or equal to width
    // and same for height
    unsigned resolution = (height > width ? height : width) / terrain_complexity;
    unsigned noise_map_width = (width/resolution +
                                    (width%resolution != 0));
    unsigned noise_map_height = (height/resolution +
                                    (height%resolution != 0));
    
    nm::Map noise_map;

    // generate a map with perlin noise
    perlin_noise_generator.generateMap(noise_map_width,
                                       noise_map_height,
                                       resolution,
                                       noise_map);
    
    for (int j = 0; j < terrain[0].size(); j++)
    {
        for (int i = 0; i < terrain.size(); i++)
        {
            if (noise_map[i][j] > qty_of_terrain/100.f)
                terrain[i][j] = gnd::Item::Water;
        }
    }

    for (int j = 0; j < terrain[0].size(); j++)
    {
        for (int i = 0; i < terrain.size(); i++)
        {
            if (randomBool((qty_of_trees/100.f)*noise_map[i][j]) and
                    not isSand(i, j))
                terrain[i][j] = gnd::Item::Tree;
        }
    }     
}


bool Terrain::randomBool (double chances)
{
    return double(rand())/RAND_MAX < chances;
}


Terrain::Terrain (std::string terrain_source_file_path, bool creatures)
{
    // try to open file
    std::ifstream file(terrain_source_file_path, std::ios::binary);

    if (not file.is_open())
    {
        std::cerr << "ERROR: could not open specified terrain file path. "
                  << "Aborting.\n";
        exit(0);
    }

    char cell;
    int counter = 0;
    
    file >> width_ >> height_;

    terrain = gnd::Map(width_, std::vector<gnd::Item>(height_));

    // read and parse file characters
    while (file >> cell)
    {
        if (counter >= width_*height_)
        {
            counter++;
            continue;
        }
        
        unsigned col = counter%width_;
        unsigned row = counter/width_;

        switch (cell)
        {
            case 'g':
                terrain[col][row] = gnd::Item::Grass;
                break;

            case 'w':
                terrain[col][row] = gnd::Item::Water;
                break;

            case 't':
                terrain[col][row] = gnd::Item::Tree;
                break;
            // if creatures == true, convert creatures to grass
            default:
            {
                if (creatures)
                {
                    terrain[col][row] = gnd::Item::Grass;
                    struct gnd::TerrainCreature creature (gnd::Point(col, row),
                                                          cell);

                    file_creatures.push_back(creature);
                }
                else
                {
                    std::cerr << "ERROR: could not parse terrain file,"
                              << " unknown character " << cell
                              << ". Aborting.\n";
                    exit(0);
                }
            }
        }

        ++counter;
    }

    // check number of read characters is correct
    if (counter != width_*height_)
    {
        std::cerr << "ERROR: unexpected number of characters, expected "
                  << (width_*height_) << " and got " << counter << ". Aborting.\n";
        exit(0);
    }
}


bool Terrain::isSand (unsigned  x, unsigned y) const
{
    return (x < width_-1  and terrain[x+1][y] == gnd::Item::Water) or
           (x > 0         and terrain[x-1][y] == gnd::Item::Water) or
           (y < height_-1 and terrain[x][y+1] == gnd::Item::Water) or
           (y > 0         and terrain[x][y-1] == gnd::Item::Water);
}


unsigned Terrain::getHeight () const
{
    return height_;
}


unsigned Terrain::getWidth () const
{
    return width_;
}


gnd::Item Terrain::getTileType (unsigned x, unsigned y) const
{
    return terrain[x][y];
}


const std::list<gnd::TerrainCreature> Terrain::getFileCreatures () const
{
    return file_creatures;
}