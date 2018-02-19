#include "GeneticAlgorithm.h"
#include <iostream>

void printResults(int fness, int iter, int cx, int ff, int ps);
bool doTraining(GeneticAlgorithm ga, int cx, int ff, int ps, int is, int ms, int mf);

int main() {
    const int INITIAL_POPULATION_SIZE = 10;
    const int MAXIMUM_POPULATION_SIZE = 1280;
    const int NUM_TRAINING_ITER = 25;
    const int MIN_TRAINING_SUCC = 24;

    // Create instance of GA
    GeneticAlgorithm ga;

    int crossoverOperator = ga.uniformX;
    int fitnessFunction = ga.countingOnes;
    int populationSize = INITIAL_POPULATION_SIZE;
    int maxFitness = 100;

    /* TODO: Repeat each experiment 25x for each population size
       TODO: and repeat that with population size change rules
       TODO: until 24/25 runs return optimal solution*/

    // Experiment 1
    bool res = doTraining(ga, crossoverOperator, fitnessFunction, populationSize, NUM_TRAINING_ITER, MIN_TRAINING_SUCC, maxFitness);

    // If failure --> double popsize and try again.
    while(!res && populationSize*2 <= MAXIMUM_POPULATION_SIZE){
        populationSize*=2;
        res = doTraining(ga, crossoverOperator, fitnessFunction, populationSize,NUM_TRAINING_ITER,MIN_TRAINING_SUCC,maxFitness);
    }
    if(populationSize*2 > MAXIMUM_POPULATION_SIZE){std::cout<<std::endl<<"Finished training"<<std::endl;}
    else{
        //Perform bisection search to find minimal value of populationSize
    }

    // TODO: Experiments 2 - 5

    return 0;
}

bool doTraining(GeneticAlgorithm ga, int cx, int ff, int ps, int is, int ms, int mf){
    int count = 0;
    // Rep 25 times
    for(int n = 0; n < is; n++){
        bool* bestSolutionFound = ga.run(cx, ff, ps);
        //Compute fitness value for bestSolution
        int fitness = 404;

        // Write fitness value
        printResults(fitness, n, cx,ff,ps);

        //Add to amount of successes
        if(fitness == mf){count++;}
    }
    return(count >= ms);
}

void printResults(int fness, int iter, int cx, int ff, int ps){
    std::cout   <<"fitness;"    <<fness
                <<";iteration;" <<iter
                <<";xOP;"      <<cx
                <<";fFun;"      <<ff
                <<";popSize;"   <<ps
                <<std::endl;
}