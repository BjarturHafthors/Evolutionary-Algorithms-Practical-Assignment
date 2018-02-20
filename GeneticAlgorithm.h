//
// Created by Bjartur on 17/02/2018.
//

#ifndef PRACTICAL1_GENETICALGORITHM_H
#define PRACTICAL1_GENETICALGORITHM_H

#include <algorithm>
#include "Individual.h"


class GeneticAlgorithm {
public:
    // VARIABLES

    // Used to determine which CO to use
    enum crossoverOperator{uniformX, twoPointX};

    // Used to determine which FF to use
    enum fitnessFunction{countingOnes,
        tightlyDeceptiveTrap,
        tightlyNonDeceptiveTrap,
        randomlyDeceptiveTrap,
        randomlyNonDeceptiveTrap};

    // FUNCTIONS

    // Performs a single run of the algorithm with specified
    // crossover operator, fitness function and population size
    std::vector<bool> run(crossoverOperator co, fitnessFunction ff, int population);

private:
    // FUNCTIONS

    void initializePopulation();
    void deletePopulation();
    void setIndividualFitness(Individual* individual);
    void selectIndividualsForNextGeneration();
    void performUniformCrossover();
    void performTwoPointCrossover();
    int countingOnesFitnessCalculation(Individual individual);
    int tightlyDeceptiveTrapFitnessCalculation(Individual individual);
    int tightlyNonDeceptiveTrapFitnessCalculation(Individual individual);
    int randomlyDeceptiveTrapFitnessCalculation(Individual individual);
    int randomlyNonDeceptiveTrapFitnessCalculation(Individual individual);

    // VARIABLES

    std::vector<Individual*> population;
    std::vector<Individual*> children;
    const int FAMILY_SIZE = 4;
    int populationSize;
    std::random_device random;
    std::mt19937 mt;
    fitnessFunction fitnessFunc;
};


#endif //PRACTICAL1_GENETICALGORITHM_H
