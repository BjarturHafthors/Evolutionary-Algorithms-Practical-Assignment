#include "GeneticAlgorithm.h"
#include <iostream>

void printResults(int fness, int iter, int cx, int ff, int ps);

int main() {
    const int INITIAL_POPULATION_SIZE = 10;
    const int NUM_TRAINING_ITER = 25;
    const int MIN_TRAINING_SUCC = 24;

    // Create instance of GA
    GeneticAlgorithm ga;

    int crossoverOperator = ga.uniformX;
    int fitnessFunction = ga.countingOnes;
    int populationSize = INITIAL_POPULATION_SIZE;
    int succ = 0;
    int maxFitness = 100;

    /* TODO: Repeat each experiment 25x for each population size
       TODO: and repeat that with population size change rules
       TODO: until 24/25 runs return optimal solution*/

    // Experiment 1
    // Rep 25 times
    for(int n = 0;n<NUM_TRAINING_ITER;n++){
        bool* bestSolutionFound = ga.run(crossoverOperator, fitnessFunction, populationSize);
        // Write fitness value
        printResults(404, n, crossoverOperator,fitnessFunction,populationSize);
        // Add to amountOfSuccesses
        if(404 > maxFitness){succ++;}
    }
    // If failure --> double popsize and try again.
    if(succ >= MIN_TRAINING_SUCC){}
    // If success --> decrease popsize and try again
    else{}

    // TODO: Experiments 2 - 5

    return 0;
}

void printResults(int fness, int iter, int cx, int ff, int ps){
    std::cout   <<"fitness;"    <<fness
                <<";iteration;" <<iter
                <<";cxOP;"      <<cx
                <<";fFun;"      <<ff
                <<";popSize;"   <<ps
                <<std::endl;
}