//
// Created by Bjartur on 20/02/2018.
//

#include "Individual.h"

Individual::Individual(bool currentlyInPopulation)
{
    this->currentlyInPopulation = currentlyInPopulation;

    // Initializing random generator
    mt = std::mt19937(random());

    if (currentlyInPopulation)
    {
        initializeValues();
    }
}

Individual::Individual(const Individual &individual)
{
    this->currentlyInPopulation = individual.isCurrentlyInPopulation();
    this->setValues(individual.getValues());
    this->setFitness(individual.getFitness());
}

Individual::Individual()
{

}

void Individual::initializeValues()
{
    // Initialize random distribution
    std::uniform_int_distribution<int> distribution(0,1);

    values = std::vector<bool>();

    for (int i = 0; i < STRING_LENGTH; i++)
    {
        // Giving each individual random initial values
        values.push_back((bool) distribution(mt));
    }
}

bool Individual::isCurrentlyInPopulation() const
{
    return currentlyInPopulation;
}

void Individual::setCurrentlyInPopulation(bool currentlyInPopulation)
{
    Individual::currentlyInPopulation = currentlyInPopulation;
}

int Individual::getFitness() const
{
    return fitness;
}

void Individual::setFitness(int fitness)
{
    Individual::fitness = fitness;
}

const std::vector<bool> &Individual::getValues() const
{
    return values;
}

void Individual::setValues(const std::vector<bool> &values)
{
    Individual::values = values;
}

bool operator<(const Individual &i1, const Individual &i2){

    if (i1.getFitness() < i2.getFitness())
    {
        return true;
    }
    // Making sure we sub-sort such that children appear before parents
    else if (i1.getFitness() == i2.getFitness() && i1.isCurrentlyInPopulation() && !i2.isCurrentlyInPopulation())
    {
        return true;
    }
    else
    {
        return false;
    }
}

Individual& Individual::operator=(const Individual& individual)
{
// check for self-assignment
    if (&individual == this)
        return *this;

    this->currentlyInPopulation = individual.isCurrentlyInPopulation();
    this->setValues(individual.getValues());
    this->setFitness(individual.getFitness());

    return *this;
}