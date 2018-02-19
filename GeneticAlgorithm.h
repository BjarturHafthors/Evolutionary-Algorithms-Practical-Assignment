//
// Created by Bjartur on 17/02/2018.
//

#ifndef PRACTICAL1_GENETICALGORITHM_H
#define PRACTICAL1_GENETICALGORITHM_H

#include <random>

class GeneticAlgorithm {
private:
    // FUNCTIONS
    void initializePopulation();
    void deletePopulation();

    // VARIABLES
    bool** population;
    const int STRING_LENGTH = 100;
    int populationSize;
    std::random_device random;
    std::mt19937 mt;

public:
    // FUNCTIONS

    // Performs a single run of the algorithm with specified
    // crossover operator, fitness function and population size
    bool* run(int co, int ff, int population);

    // VARIABLES

    // Used to determine which CO to use
    enum crossoverOperator{uniformX, twoPointX};

    // Used to determine which FF to use
    enum fitnessFunction{countingOnes,
        tightlyDeceptiveTrap,
        tightlyNonDeceptiveTrap,
        randomlyDeceptiveTrap,
        randomlyNonDeceptiveTrap};
};


#endif //PRACTICAL1_GENETICALGORITHM_H
