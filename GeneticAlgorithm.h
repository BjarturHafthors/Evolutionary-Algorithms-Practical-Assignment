//
// Created by Bjartur on 17/02/2018.
//

#ifndef PRACTICAL1_GENETICALGORITHM_H
#define PRACTICAL1_GENETICALGORITHM_H

#include <algorithm>
#include "Individual.h"
#include "Enums.h"
#include <math.h>
#include <iostream>


class GeneticAlgorithm {
public:
    // VARIABLES

    int generationCount;
    float averageGenerationCountOfLastSucces;
    float averageRuntimeOfLastSucces;
    int numberOfFitFnEvalualtions;
    float averageNumberOfFitFnEvaluations;

    // FUNCTIONS

    // Performs a single run of the algorithm with specified
    // crossover operator, fitness function and population size
    float run(CrossoverOperator co, FitnessFunction ff, int population, bool metricRun);
    GeneticAlgorithm(GeneticAlgorithm &ga);
    GeneticAlgorithm();

private:
    // FUNCTIONS

    void initializePopulation();
    void deletePopulation();
    void setIndividualFitness(Individual* individual);
    void selectIndividualsForNextGeneration();
    float findBestSolution();
    void performUniformCrossover();
    void performTwoPointCrossover();
    float countingOnesFitnessCalculation(Individual* individual);
    float tightlyLinkedTrapFitnessCalculation(int k, float d, Individual* individual);
    float randomlyLinkedTrapFitnessCalculation(int k, float d, Individual* individual);
    void printMetrics();
    void checkSelectionErrors(Individual parentStaying, Individual parentLosing, Individual child);

    // VARIABLES

    std::vector<Individual*> population;
    std::vector<Individual*> children;
    int populationSize;
    std::random_device random;
    std::mt19937 mt;
    FitnessFunction fitnessFunc;
    bool aChildWasAddedToThePopulation;
    std::vector<int> permutation;
    std::uniform_int_distribution<int> distribution;
    int selectionErrors;
    bool metricRun;
};


#endif //PRACTICAL1_GENETICALGORITHM_H
