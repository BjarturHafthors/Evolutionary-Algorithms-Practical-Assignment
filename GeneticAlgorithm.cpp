//
// Created by Bjartur on 17/02/2018.
//

#include "GeneticAlgorithm.h"
#include <iostream>

GeneticAlgorithm::GeneticAlgorithm()
{

}

GeneticAlgorithm::GeneticAlgorithm(GeneticAlgorithm &ga)
{
    this->population = ga.population;
    this->children = ga.children;
    this->populationSize = ga.populationSize;
    this->mt = ga.mt;
}

int GeneticAlgorithm::run(CrossoverOperator co, FitnessFunction ff, int pSize)
{
    fitnessFunc = ff;
    this->generationCount = 0;
    this->numberOfFitFnEvalualtions = 0;

    // Initializing random generator
    mt = std::mt19937(random());

    // Initializing population
    this->populationSize = pSize;
    initializePopulation();

    // Set random permutations if needed
    if (fitnessFunc == randomlyDeceptiveTrap || fitnessFunc == randomlyNonDeceptiveTrap)
    {
        permutation = std::vector<int>();

        for (int i = 0; i < population.size(); i++)
        {
            permutation.push_back(i);
        }

        // Randomizing
        std::shuffle(permutation.begin(), permutation.end(), mt);
    }

    // Repeat until children are not used any more
    do
    {
        // Reset condition
        aChildWasAddedToThePopulation = false;
        this->generationCount += 1 ;

        // Randomly shuffle population
        std::shuffle(population.begin(), population.end(), mt);

        // Pick appropriate crossover function
        switch (co)
        {
            case CrossoverOperator::uniformX:
                performUniformCrossover();
                break;
            case CrossoverOperator::twoPointX:
                performTwoPointCrossover();
                break;
        }

        selectIndividualsForNextGeneration();

    }
    while (aChildWasAddedToThePopulation);

    int bestSolutionFitness = findBestSolution();

    deletePopulation();

    // Return best solution found
    return bestSolutionFitness;
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
        delete population[i];
    }

    // Deallocate memory for population
    for (int i = 0; i < children.size(); i++)
    {
        delete children[i];
    }
}

void GeneticAlgorithm::setIndividualFitness(Individual* individual)
{
    int fitness = -1;

    // Pick correct fitness function
    switch(fitnessFunc)
    {
        case FitnessFunction::countingOnes:
            fitness = countingOnesFitnessCalculation(individual);
            break;
        case FitnessFunction::tightlyDeceptiveTrap:
            // Values based on project description - k and d respectively
            fitness = tightlyLinkedTrapFitnessCalculation(4, 1, individual);
            break;
        case FitnessFunction::tightlyNonDeceptiveTrap:
            // Values based on project description - k and d respectively
            fitness = tightlyLinkedTrapFitnessCalculation(4, 2.5, individual);
            break;
        case FitnessFunction::randomlyDeceptiveTrap:
            // Values based on project description - k and d respectively
            fitness = randomlyLinkedTrapFitnessCalculation(4, 1, individual);
            break;
        case FitnessFunction::randomlyNonDeceptiveTrap:
            // Values based on project description - k and d respectively
            fitness = randomlyLinkedTrapFitnessCalculation(4, 2.5, individual);
            break;
    }

    individual->setFitness(fitness);
}

void GeneticAlgorithm::selectIndividualsForNextGeneration()
{
    for (int i = 0; i < populationSize - 1; i += 2)
    {
        // Get family
        std::vector<Individual> family = std::vector<Individual>();
        family.emplace_back(*population[i]);
        family.emplace_back(*population[i + 1]);
        family.emplace_back(*children[i]);
        family.emplace_back(*children[i + 1]);

        // Sort family by fitness - subsorted by
        // whether child or parent (childs appear first)
        std::sort(family.rbegin(), family.rend());

        // If either is child (if this does not apply we won't update anything)
        if (!family[0].isCurrentlyInPopulation() || !family[1].isCurrentlyInPopulation())
        {
            // First is already in population
            if (family[0].isCurrentlyInPopulation() && !family[1].isCurrentlyInPopulation())
            {
                // If population[i] is the parent, update population[i+1]
                if (population[i]->getValues() == family[0].getValues())
                {
                    population[i + 1]->setFitness(family[1].getFitness());
                    population[i + 1]->setValues(family[1].getValues());
                }
                    // If population[i+1] is the parent, update population[i]
                else
                {
                    population[i]->setFitness(family[1].getFitness());
                    population[i]->setValues(family[1].getValues());
                }
            }
                // Second is already in population
            else if (family[1].isCurrentlyInPopulation() && !family[0].isCurrentlyInPopulation())
            {
                // If population[i] is the parent, update population[i+1]
                if (population[i]->getValues() == family[1].getValues())
                {
                    population[i+1]->setFitness(family[0].getFitness());
                    population[i+1]->setValues(family[0].getValues());
                }
                    // If population[i+1] is the parent, update population[i]
                else
                {
                    population[i]->setFitness(family[0].getFitness());
                    population[i]->setValues(family[0].getValues());
                }
            }
                // Both are children
            else
            {
                // Update both
                population[i]->setFitness(family[0].getFitness());
                population[i]->setValues(family[0].getValues());
                population[i+1]->setFitness(family[1].getFitness());
                population[i+1]->setValues(family[1].getValues());
            }

            bool allTheSame = family[0].getValues() == family[1].getValues() &&
                              family[0].getValues() == family[2].getValues() &&
                              family[0].getValues() == family[3].getValues();

            // Update while condition if not all some solutions are the same
            if (!aChildWasAddedToThePopulation && !allTheSame)
            {
                aChildWasAddedToThePopulation = true;
            }
        }
    }
}

int GeneticAlgorithm::findBestSolution()
{
    int highestFitnessFound = -1;

    for (int i = 0; i < population.size(); i++)
    {
        if (population[i]->getFitness() > highestFitnessFound)
        {
            highestFitnessFound = population[i]->getFitness();
        }
    }

    return highestFitnessFound;
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
            if(parent1Values[j] == parent2Values[j]){
                child1Values.push_back(parent1Values[j]);
                child2Values.push_back(parent1Values[j]);
            }
            else{
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
    // Initialize random distribution
    std::uniform_int_distribution<int> distribution(1, 98);

    int point1 = distribution(mt);
    int point2 = distribution(mt);

    while (point2 == point1)
    {
        point2 = distribution(mt);
    }

    for (int i = 0; i < population.size() - 1; i += 2)
    {
        std::vector<bool> child1Values = std::vector<bool>();
        std::vector<bool> child2Values = std::vector<bool>();
        std::vector<bool> parent1Values = population[i]->getValues();
        std::vector<bool> parent2Values = population[i + 1]->getValues();

        for (int j = 0; j < parent1Values.size(); j++)
        {
            if (j < point1 || j > point2)
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
        children[i + 1]->setValues(child2Values);

        // Save fitness in individuals
        setIndividualFitness(children[i]);
        setIndividualFitness(children[i + 1]);
    }
}

int GeneticAlgorithm::countingOnesFitnessCalculation(Individual* individual)
{
    std::vector<bool> values = individual->getValues();

    int fitness = 0;

    for (int i = 0; i < values.size(); i++)
    {
        if (values[i])
        {
            fitness++;
        }
    }

    this->numberOfFitFnEvalualtions++;
    return fitness;
}

int GeneticAlgorithm::tightlyLinkedTrapFitnessCalculation(int k, float d, Individual* individual)
{
    float fraction = (k-d)/(k-1);
    std::vector<bool> values = individual->getValues();

    int fitness = 0;

    for (int i = 0; i < values.size()/k; i++)
    {
        int numberOfOnes = 0;

        for (int j = 0; j < k; j++)
        {
            if (values[i+j])
            {
                numberOfOnes++;
            }
        }

        if (numberOfOnes == k)
        {
            fitness += k;
        }
        else
        {
            fitness += k - d - fraction*numberOfOnes;
        }
    }

    this->numberOfFitFnEvalualtions++;
    return fitness;
}

int GeneticAlgorithm::randomlyLinkedTrapFitnessCalculation(int k, float d, Individual* individual)
{
    float fraction = (k-d)/(k-1);
    std::vector<bool> values = individual->getValues();

    int fitness = 0;

    for (int i = 0; i < values.size()/k; i++)
    {
        int numberOfOnes = 0;

        for (int j = 0; j < k; j++)
        {
            if (values[permutation[i]])
            {
                numberOfOnes++;
            }
        }

        if (numberOfOnes == k)
        {
            fitness += k;
        }
        else
        {
            fitness += k - d - fraction*numberOfOnes;
        }
    }

    this->numberOfFitFnEvalualtions++;
    return fitness;
}