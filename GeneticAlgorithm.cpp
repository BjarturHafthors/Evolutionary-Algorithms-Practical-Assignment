//
// Created by Bjartur on 17/02/2018.
//

#include "GeneticAlgorithm.h"

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

float GeneticAlgorithm::run(CrossoverOperator co, FitnessFunction ff, int pSize, bool metricRun)
{
    fitnessFunc = ff;
    this->generationCount = 0;
    this->numberOfFitFnEvalualtions = 0;
    selectionErrors = 0;

    // Initializing random generator
    mt = std::mt19937(random());

    switch (co)
    {
        case uniformX:
            distribution = std::uniform_int_distribution<int>(0,1);
            break;
        case twoPointX:
            distribution = std::uniform_int_distribution<int>(1,98);
    }

    // Set random permutations if needed
    if (fitnessFunc == randomlyDeceptiveTrap || fitnessFunc == randomlyNonDeceptiveTrap)
    {
        permutation = std::vector<int>();

        for (int i = 0; i < Individual::STRING_LENGTH; i++)
        {
            permutation.push_back(i);
        }

        // Randomizing
        std::shuffle(permutation.begin(), permutation.end(), mt);
    }

    // Initializing population
    this->populationSize = pSize;
    initializePopulation();

    printMetrics();

    // Repeat until children are not used any more
    do
    {
        // Reset condition
        aChildWasAddedToThePopulation = false;
        this->generationCount += 1 ;
        selectionErrors = 0;

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

        if (metricRun)
        {
            printMetrics();
        }
    }
    while (aChildWasAddedToThePopulation);

    float bestSolutionFitness = findBestSolution();

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

                    checkSelectionErrors(population[i], population[i+1], family[1]);
                }
                // If population[i+1] is the parent, update population[i]
                else
                {
                    population[i]->setFitness(family[1].getFitness());
                    population[i]->setValues(family[1].getValues());

                    checkSelectionErrors(population[i+1], population[i], family[1]);
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

                    checkSelectionErrors(population[i], population[i+1], family[0]);
                }
                // If population[i+1] is the parent, update population[i]
                else
                {
                    population[i]->setFitness(family[0].getFitness());
                    population[i]->setValues(family[0].getValues());

                    checkSelectionErrors(population[i+1], population[i], family[0]);
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

            bool allTheSame = family[0].getFitness() == family[1].getFitness() &&
                              family[0].getFitness() == family[2].getFitness() &&
                              family[0].getFitness() == family[3].getFitness();

            // Update while condition if not all some solutions are the same
            if (!aChildWasAddedToThePopulation && !allTheSame)
            {
                aChildWasAddedToThePopulation = true;
            }
        }
    }
}

float GeneticAlgorithm::findBestSolution()
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
    for (int i = 0; i < population.size() - 1; i += 2)
    {

        int point1 = distribution(mt);
        int point2 = distribution(mt);

        while (point2 == point1)
        {
            point2 = distribution(mt);
        }

        // Make sure point1 is lower
        if (point1 > point2)
        {
            // Swap
            int temp = point1;
            point1 = point2;
            point2 = temp;
        }
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

float GeneticAlgorithm::countingOnesFitnessCalculation(Individual* individual)
{
    std::vector<bool> values = individual->getValues();

    float fitness = 0;

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

float GeneticAlgorithm::tightlyLinkedTrapFitnessCalculation(int k, float d, Individual* individual)
{
    float fraction = (k-d)/(k-1);
    std::vector<bool> values = individual->getValues();

    float fitness = 0;

    for (int i = 0; i < values.size()/k; i++)
    {
        int numberOfOnes = 0;

        for (int j = 0; j < k; j++)
        {
            if (values[i*k+j])
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

float GeneticAlgorithm::randomlyLinkedTrapFitnessCalculation(int k, float d, Individual* individual)
{
    float fraction = (k-d)/(k-1);
    std::vector<bool> values = individual->getValues();

    float fitness = 0;

    for (int i = 0; i < values.size()/k; i++)
    {
        int numberOfOnes = 0;

        for (int j = 0; j < k; j++)
        {
            if (values[permutation[k*i+j]])
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

void GeneticAlgorithm::checkSelectionErrors(Individual parentStaying, Individual parentLosing, Individual child)
{
    std::vector<bool> stayingValues = parentStaying.getValues();
    std::vector<bool> losingValues = parentLosing.getValues();
    std::vector<bool> childValues = child.getValues();

    for (int i = 0; i < Individual::STRING_LENGTH; i++)
    {
        if (!stayingValues[i] && !childValues[i] && losingValues[i])
        {
            selectionErrors++;
        }
    }
}

void GeneticAlgorithm::printMetrics()
{
    float fitnessSum;
    for (int i = 0; i < population.size(); i++)
    {
        fitnessSum += population[i]->getFitness();
    }

    float averageFitness = fitnessSum/population.size();
    float proportion = fitnessSum/(population.size()*100);

    float sum = 0;

    for (int i = 0; i < population.size(); i++)
    {
        sum += std::pow((population[i]->getFitness() - averageFitness), 2);
    }

    sum /= population.size();

    float standardDeviation = sqrt(sum);

    std::cout << "Generation: " << generationCount << std::endl;
    std::cout << "Proportion: " << proportion << std:: endl;
    std::cout << "Avg Fitness: " << averageFitness << std::endl;
    std::cout << "Standard Deviation: " <<  standardDeviation << std::endl;
    std::cout << "Selection Errors: " << selectionErrors << std::endl << std::endl;
}