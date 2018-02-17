#include "GeneticAlgorithm.h"

int main() {
    const int INITIAL_POPULATION_SIZE = 10;

    // Create instance of GA
    GeneticAlgorithm ga;

    int crossoverOperator = ga.uniformX;
    int fitnessFunction = ga.countingOnes;
    int populationSize = INITIAL_POPULATION_SIZE;

    /* TODO: Repeat each experiment 25x for each population size
       TODO: and repeat that with population size change rules
       TODO: until 24/25 runs return optimal solution*/

    // Experiment 1
    bool* bestSolutionFound = ga.run(crossoverOperator, fitnessFunction, populationSize);

    // TODO: Experiments 2 - 5

    return 0;
}