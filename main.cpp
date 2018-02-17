#include <iostream>
#include "GeneticAlgorithm.h"

int main() {
    const int INITIAL_POPULATION_SIZE = 10;

    // Create instance of GA
    GeneticAlgorithm ga;

    int crossoverOperator = ga.uniformX;
    int fitnessFunction = ga.countingOnes;
    int populationSize = INITIAL_POPULATION_SIZE;

    // Experiment 1
    ga.run(crossoverOperator, fitnessFunction, populationSize);

    
    return 0;
}