#ifndef NOISEMAP_H_
#define NOISEMAP_H_

#include <vector>


namespace nm
{
    typedef std::vector<std::vector<double>> Map;
    typedef std::pair<double, double> Dot;
    class NoiseMap;

    // returns true with a probability of chances
    // (chances must be between 0 and 1)
    bool randomBool (double chances);

    double enhancedFade (double p1, double p2, double x);
    double linearFade   (double p1, double p2, double x);
}


class nm::NoiseMap
{
public:

    // it is recomended to set a seed the first time the class is used
    NoiseMap ();

    // Stores the result in the map vector. Resolution means the number of
    // sub squares in each sector. The returned map will be of size
    // width*resolution x height*resolution
    void generateMap (unsigned width,
                      unsigned height,
                      unsigned resolution,
                      Map &map);
    
    void setFadeFunction (double (*fadeFunc)(double,double,double));

    // Set seed for the random generation of dots in the map
    static void setSeed (unsigned seed);

    static void setRandomSeed ();

private:

    static Dot getVector (Dot vertex, unsigned x, unsigned y, unsigned res);
    static double dotProduct (Dot d1, Dot d2);

    double (*fadeFunction)(double,double,double);
};


#endif