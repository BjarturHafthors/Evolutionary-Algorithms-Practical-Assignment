//
// Created by Bjartur on 20/02/2018.
//

#ifndef PRACTICAL1_INDIVIDUAL_H
#define PRACTICAL1_INDIVIDUAL_H


#include <random>
#include <vector>

class Individual
{
public:
    static const int STRING_LENGTH = 100;
    Individual(bool currentlyInPopulation);
    Individual(const Individual &individual);
    Individual();
    void initializeValues();
    bool isCurrentlyInPopulation() const;
    void setCurrentlyInPopulation(bool currentlyInPopulation);
    int getFitness() const;
    void setFitness(int fitness);
    const std::vector<bool> &getValues() const;
    void setValues(const std::vector<bool> &values);
    Individual& operator=(const Individual& other);

private:
    std::vector<bool> values;
    bool currentlyInPopulation;
    int fitness;
    std::random_device random;
    std::mt19937 mt;
};

// Overload < operator so we can use std::sort
bool operator<(const Individual &i1, const Individual &i2);

#endif //PRACTICAL1_INDIVIDUAL_H
