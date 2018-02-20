//
// Created by Bjartur on 17/02/2018.
//

#include "GeneticAlgorithm.h"

std::vector<bool> GeneticAlgorithm::run(crossoverOperator co, fitnessFunction ff, int pSize)
{
    fitnessFunc = ff;

    // Initializing random generator
    mt = std::mt19937(random());

    // Initializing population
    this->populationSize = pSize;
    initializePopulation();

    bool aChildWasAddedToThePopulation = true;

    // Repeat until children are not used any more
    do
    {
        // Randomly shuffle population
        std::shuffle(population.begin(), population.end(), mt);

        // Pick appropriate crossover function
        switch (co)
        {
            case uniformX:
                performUniformCrossover();
                break;
            case twoPointX:
                performTwoPointCrossover();
                break;
        }

        selectIndividualsForNextGeneration();

        // TODO: Update condition (aChildWasAdded)

    }
    while (aChildWasAddedToThePopulation);

    deletePopulation();

    // Return best solution found
}

void GeneticAlgorithm::initializePopulation()
{
   // Allocate memory for population
    population = std::vector<Individual*>();

    for (int i = 0; i < populationSize; i++)
    {
        // Pass true as these individuals go directly
        // into the population
        population.push_back(new Individual(true));
    }

    // Allocate memory for children
    children = std::vector<Individual*>();

    // Fill vector with individuals
    for (int i = 0; i < populationSize; i++)
    {
        // Pass false as these individuals don't go directly
        // into the population
        children.push_back(new Individual(false));
    }

    // Calculate fitness for each individual and save it
    for (int i = 0; i < population.size(); i++)
    {
        setIndividualFitness(population[i]);
    }
}

void GeneticAlgorithm::deletePopulation()
{
    // Deallocate memory for population
    for (int i = 0; i < population.size(); i++)
    {
        delete [] population[i];
    }

    // Deallocate memory for population
    for (int i = 0; i < children.size(); i++)
    {
        delete [] children[i];
    }
}

void GeneticAlgorithm::setIndividualFitness(Individual* individual)
{
    int fitness = -1;

    // Pick correct fitness function
    switch(fitnessFunc)
    {
        case countingOnes:
            fitness = countingOnesFitnessCalculation(individual);
            break;
        case tightlyDeceptiveTrap:
            fitness = tightlyDeceptiveTrapFitnessCalculation(individual);
            break;
        case tightlyNonDeceptiveTrap:
            fitness = tightlyNonDeceptiveTrapFitnessCalculation(individual);
            break;
        case randomlyDeceptiveTrap:
            fitness = randomlyDeceptiveTrapFitnessCalculation(individual);
            break;
        case randomlyNonDeceptiveTrap:
            fitness = randomlyNonDeceptiveTrapFitnessCalculation(individual);
            break;
    }

    individual->setFitness(fitness);
}

void GeneticAlgorithm::selectIndividualsForNextGeneration()
{
    // TODO: Pick best 2 solutions of each family of 4

    for (int i = 0; i < populationSize - 1; i += 2)
    {
        // Get family
        std::vector<Individual> family = std::vector<Individual>();
        family.push_back(population[i]);
        family.push_back(population[i + 1]);
        family.push_back(children[i]);
        family.push_back(children[i + 1]);

        // Sort family by fitness - subsorted by
        // whether child or parent (childs appear first)
        std::sort(family.begin(), family.end());

        bool bothChildren = !family[0].isCurrentlyInPopulation() && !family[1].isCurrentlyInPopulation;
        bool firstChild = !family[0].isCurrentlyInPopulation() && family[1].isCurrentlyInPopulation;
        bool secondChild = family[0].isCurrentlyInPopulation() && !family[1].isCurrentlyInPopulation;

        // Both children
        if (bothChildren)
        {
            population[i]->setFitness(family[0].getFitness());
            population[i]->setValues(family[0].getValues());
            population[i]->setFitness(family[1].getFitness());
        }
    }
}
void GeneticAlgorithm::performUniformCrossover()
{
    // Initialize random distribution
    std::uniform_int_distribution<int> distribution(0,1);

    for (int i = 0; i < population.size()-1; i+=2)
    {
        std::vector<bool> child1Values = std::vector<bool>();
        std::vector<bool> child2Values = std::vector<bool>();
        std::vector<bool> parent1Values = population[i]->getValues();
        std::vector<bool> parent2Values = population[i+1]->getValues();

        for (int j = 0; j < parent1Values.size(); j++)
        {
            if ((bool) distribution(mt))
            {
                child1Values.push_back(parent1Values[j]);
                child2Values.push_back(parent2Values[j]);
            }
            else
            {
                child1Values.push_back(parent2Values[j]);
                child2Values.push_back(parent1Values[j]);
            }
        }

        children[i]->setValues(child1Values);
        children[i+1]->setValues(child2Values);

        // Save fitness in individuals
        setIndividualFitness(children[i]);
        setIndividualFitness(children[i+1]);
    }
}

void GeneticAlgorithm::performTwoPointCrossover()
{
    // TODO: Implement..
}

int GeneticAlgorithm::countingOnesFitnessCalculation(Individual individual)
{
    // TODO: Implement..
}

int GeneticAlgorithm::tightlyDeceptiveTrapFitnessCalculation(Individual individual)
{
    // TODO: Implement..
    return 0;
}
int GeneticAlgorithm::tightlyNonDeceptiveTrapFitnessCalculation(Individual individual)
{
    // TODO: Implement..
    return 0;
}

int GeneticAlgorithm::randomlyDeceptiveTrapFitnessCalculation(Individual individual)
{
    // TODO: Implement..
    return 0;
}

int GeneticAlgorithm::randomlyNonDeceptiveTrapFitnessCalculation(Individual individual)
{
    // TODO: Implement..
    return 0;
}