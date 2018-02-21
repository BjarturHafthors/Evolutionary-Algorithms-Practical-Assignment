//
// Created by bjart on 21/02/2018.
//

#ifndef PRACTICAL1_ENUMS_H
#define PRACTICAL1_ENUMS_H


enum CrossoverOperator{uniformX, twoPointX};

// Used to determine which FF to use
enum FitnessFunction{countingOnes,
    tightlyDeceptiveTrap,
    tightlyNonDeceptiveTrap,
    randomlyDeceptiveTrap,
    randomlyNonDeceptiveTrap};

#endif //PRACTICAL1_ENUMS_H
