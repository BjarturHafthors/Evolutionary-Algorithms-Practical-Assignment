//
// Created by Bjartur on 17/02/2018.
//

#include "GeneticAlgorithm.h"

bool* GeneticAlgorithm::run(int co, int ff, int pSize)
{
    // Initializing population
    this->populationSize = pSize;
    initializePopulation();

    // Initializing random generator
    mt = std::mt19937(random());

    bool aChildWasAddedToThePopulation = true;

    // Repeat until children are not used any more
    // TODO: Ask if the teacher recommends a better stopping criterion
    while (aChildWasAddedToThePopulation)
    {
        // TODO: Randomly shuffle population for every round except first

        // TODO: Generate 2 offspring per parent pair using crossover

        // TODO: Pick best 2 solutions of each family of 4


        // TODO: Update condition (aChildWasAdded)
    }

    // Freeing population memory
    deletePopulation();


}

void GeneticAlgorithm::initializePopulation()
{
    // Initialize random distribution
    std::uniform_int_distribution<int> distribution(0,1);

    // Allocate memory for population
    population = new bool*[populationSize];
    for (int i = 0; i < populationSize; i++)
    {
        population[i] = new bool[STRING_LENGTH];
        for (int j = 0; j < STRING_LENGTH; j++)
        {
            // Giving each individual random initial values
            population[i][j] = (bool) distribution(mt);
        }
    }
}

void GeneticAlgorithm::deletePopulation()
{
    // Deallocate memory for population
    for (int i = 0; i < populationSize; i++)
    {
        delete [] population[i];
    }

    delete[] population;
}