//
// Created by Bjartur on 17/02/2018.
//

#ifndef PRACTICAL1_GENETICALGORITHM_H
#define PRACTICAL1_GENETICALGORITHM_H

#include <algorithm>
#include "Individual.h"
#include "Enums.h"


class GeneticAlgorithm {
public:
    // VARIABLES

    // FUNCTIONS

    // Performs a single run of the algorithm with specified
    // crossover operator, fitness function and population size
    int run(CrossoverOperator co, FitnessFunction ff, int population);
    GeneticAlgorithm(GeneticAlgorithm &ga);
    GeneticAlgorithm();

private:
    // FUNCTIONS

    void initializePopulation();
    void deletePopulation();
    void setIndividualFitness(Individual* individual);
    void selectIndividualsForNextGeneration();
    int findBestSolution();
    void performUniformCrossover();
    void performTwoPointCrossover();
    int countingOnesFitnessCalculation(Individual* individual);
    int tightlyTrapFitnessCalculation(int k, float d, Individual* individual);
    int randomlyDeceptiveTrapFitnessCalculation(Individual* individual);
    int randomlyNonDeceptiveTrapFitnessCalculation(Individual* individual);

    // VARIABLES

    std::vector<Individual*> population;
    std::vector<Individual*> children;
    int populationSize;
    std::random_device random;
    std::mt19937 mt;
    FitnessFunction fitnessFunc;
    bool aChildWasAddedToThePopulation;
};


#endif //PRACTICAL1_GENETICALGORITHM_H
