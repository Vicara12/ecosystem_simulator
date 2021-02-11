#include "terrain.h"


Terrain::Terrain (unsigned width, unsigned height) :
        width_(width),
        height_(height)
{
    //
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

    std::string line;
    std::list<std::string> file_content;

    // reading and parsing file data can't be done at once because the map
    // matrix of the terrain is a vector of columns, and the text file is a
    // vector of rows, so it first needs to be read and then parsed

    // read file data
    while (file.eof())
    {
        std::getline(file, line);

        // check line size is consistent
        if (not file_content.empty() and
            file_content.front().size() != line.size())
        {
            std::cerr << "ERROR: terrain file path character per line is"
                    << " inconsistent at line " << (file_content.size() + 1)
                    << ". Aborting.\n";
            exit(0);
        }

        file_content.push_back(line);
    }

    width_  = file_content.front().size();
    height_ = file_content.size();

    terrain = gnd::Map(width_, std::vector<gnd::Item>(height_));

    // parse file data
    int row = 0;
    for (auto file_line : file_content)
    {
        for (int col = 0; col < file_line.size(); col++)
        {
            switch (file_line[col])
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
                        struct gnd::TerrainCreature creature;

                        creature.pos.x = col;
                        creature.pos.y = row;
                        creature.character = file_line[col];

                        file_creatures.push_back(creature);
                    }
                    else
                    {
                        std::cerr << "ERROR: could not parse terrain file,"
                             << " unknown character " << terrain[col][row]
                             << ". Aborting.\n";
                        exit(0);
                    }
                }
            }
        }

        row++;
    }
}


bool Terrain::isSand (unsigned  x, unsigned y) const
{
    return terrain[x+1][y] == gnd::Item::Water or
           terrain[x-1][y] == gnd::Item::Water or
           terrain[x][y+1] == gnd::Item::Water or
           terrain[x][y-1] == gnd::Item::Water;
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