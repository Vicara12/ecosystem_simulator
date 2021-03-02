#include "noisemap.h"

#include <random>


nm::NoiseMap::NoiseMap () : fadeFunction(enhancedFade)
{
    //
}


void nm::NoiseMap::generateMap (unsigned width,
                                unsigned height,
                                unsigned resolution,
                                Map &map)
{
    // give the map the correct shape
    map = nm::Map(width*resolution, std::vector<double>(height*resolution));

    // vertex array
    std::vector<std::vector<Dot>> vertex(width+1, std::vector<Dot>(height+1));

    // fill with random vertices
    for (int i = 0; i < vertex.size(); i++)
        for (int j = 0; j < vertex[0].size(); j++)
            vertex[i][j] = {(rand()%2 ? 1 : -1), (rand()%2 ? 1 : -1)};
    
    // positive x (width) goes to the right and positive y (height) goes down
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            // now fill the square
            for (int w = 0; w < resolution; w++)
            {
                for (int h = 0; h < resolution; h++)
                {
                    /* vertex numbering

                            1-2
                            | |
                            3-4
                    */

                    // compute doct product for each subsquare
                    double v1 = dotProduct(vertex[i][j],
                                          getVector({0,0}, w, h, resolution));
                    double v2 = dotProduct(vertex[i+1][j],
                                          getVector({1,0}, w, h, resolution));
                    double v3 = dotProduct(vertex[i][j+1],
                                          getVector({0,1}, w, h, resolution));
                    double v4 = dotProduct(vertex[i+1][j+1],
                                          getVector({1,1}, w, h, resolution));
                    
                    double n1 = fadeFunction(v1, v2, (1.f/resolution)*(w+0.5));
                    double n2 = fadeFunction(v3, v4, (1.f/resolution)*(w+0.5));
                    double result = fadeFunction(n1, n2, (1.f/resolution)*(h+0.5));
                    
                    // convert result from [1,-1] to [1,0] and store it
                    map[i*resolution + w][j*resolution + h] = (result + 1)/2.f;
                }
            }
        }
    }
}


void nm::NoiseMap::setFadeFunction (double (*fadeFunc)(double,double,double))
{
    fadeFunction = fadeFunc;
}


void nm::NoiseMap::setSeed (unsigned seed)
{
    srand(seed);
}


void nm::NoiseMap::setRandomSeed ()
{
    srand(clock());
}


nm::Dot nm::NoiseMap::getVector (Dot vertex, unsigned x, unsigned y, unsigned res)
{
    // get coordinates of the center of the sub square
    float cell_x = (1.f/res)*(x+0.5);
    float cell_y = (1.f/res)*(y+0.5);

    return {cell_x-vertex.first, cell_y-vertex.second};
}


double nm::NoiseMap::dotProduct (Dot d1, Dot d2)
{
    return d1.first*d2.first + d1.second*d2.second;
}


double nm::enhancedFade (double p1, double p2, double x)
{
    double value = (6*pow(x,5) - 15*pow(x,4) + 10*pow(x,3));

    return (p2-p1)*value + p1;
}


double nm::linearFade (double p1, double p2, double x)
{
    return (p2-p1)*x + p1;
}


bool nm::randomBool (double chances)
{
    return double(rand())/RAND_MAX < chances;
}