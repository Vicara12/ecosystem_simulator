#include "ecosystemsimulator.h"

int main (int argc, char** argv)
{
    while (true)
    {
    EcosystemSimulator ecosistem_simulator(argc, argv);
    
    ecosistem_simulator.run();
    }
}