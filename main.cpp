#include "GeneticAlgorithm.h"
#include <iostream>
#include <time.h>

void printResults(int fness, int iter, int cx, int ff, int ps);
void runExperiment(GeneticAlgorithm &ga, CrossoverOperator crossoverOperator, FitnessFunction fitnessFunction);
bool doTraining(GeneticAlgorithm &ga, CrossoverOperator cx, FitnessFunction ff, int ps, int is, int ms, int mf);

int main() {
    std::cout << "Starting Program" << std::endl << std::endl;

    // Create instance of GA
    GeneticAlgorithm ga;

    std::cout << "Starting Experiment 1" << std::endl << std::endl;
    std::cout << "Uniform Crossover + Counting Ones" << std::endl << std::endl;
    runExperiment(ga, CrossoverOperator::uniformX, FitnessFunction::countingOnes);
    std::cout << "Two Point Crossover + Counting Ones" << std::endl << std::endl;
    runExperiment(ga, CrossoverOperator::twoPointX, FitnessFunction::countingOnes);

    std::cout << "Starting Experiment 2" << std::endl << std::endl;
    std::cout << "Uniform Crossover + Tightly Linked Deceptive Trap Function" << std::endl << std::endl;
    runExperiment(ga, CrossoverOperator::uniformX, FitnessFunction::tightlyDeceptiveTrap);
    std::cout << "Two Point Crossover + Tightly Linked Deceptive Trap Function" << std::endl << std::endl;
    runExperiment(ga, CrossoverOperator::twoPointX, FitnessFunction::tightlyDeceptiveTrap);

    std::cout << "Starting Experiment 3" << std::endl << std::endl;
    std::cout << "Uniform Crossover + Tightly Linked Non-Deceptive Trap Function" << std::endl << std::endl;
    runExperiment(ga, CrossoverOperator::uniformX, FitnessFunction::tightlyNonDeceptiveTrap);
    std::cout << "Two Point Crossover + Tightly Linked Non-Deceptive Trap Function" << std::endl << std::endl;
    runExperiment(ga, CrossoverOperator::twoPointX, FitnessFunction::tightlyNonDeceptiveTrap);

    std::cout << "Starting Experiment 4" << std::endl << std::endl;
    std::cout << "Uniform Crossover + Randomly Linked Deceptive Trap Function" << std::endl << std::endl;
    runExperiment(ga, CrossoverOperator::uniformX, FitnessFunction::randomlyDeceptiveTrap);
    std::cout << "Two Point Crossover + Randomly Linked Deceptive Trap Function" << std::endl << std::endl;
    runExperiment(ga, CrossoverOperator::twoPointX, FitnessFunction::randomlyDeceptiveTrap);

    std::cout << "Starting Experiment 5" << std::endl << std::endl;
    std::cout << "Uniform Crossover + Randomly Linked Non-Deceptive Trap Function" << std::endl << std::endl;
    runExperiment(ga, CrossoverOperator::uniformX, FitnessFunction::randomlyNonDeceptiveTrap);
    std::cout << "Two Point Crossover + Randomly Linked Non-Deceptive Trap Function" << std::endl << std::endl;
    runExperiment(ga, CrossoverOperator::twoPointX, FitnessFunction::randomlyNonDeceptiveTrap);

    return 0;
}

bool doTraining(GeneticAlgorithm &ga, CrossoverOperator cx, FitnessFunction ff, int ps, int it, int ms, float mf){
    std::cout << "Trying with population size = " << ps << ": ";
    std::cout.flush();
    int count = 0; int generations = 0; int fFEvals = 0;

    //Init clock
    clock_t t = clock();

    float bestFound = 0;

    // Rep 25 times
    for(int n = 0; n < it; n++){
        //Run the training step
        float bestSolutionFitness = ga.run(cx, ff, ps);

        //Add to number of generations and number of fitness function evaluations
        generations += ga.generationCount;
        fFEvals += ga.numberOfFitFnEvalualtions;

        //Add to number of successes
        if(bestSolutionFitness == mf){count++;}

        if (bestSolutionFitness > bestFound)
        {
            bestFound = bestSolutionFitness;
        }
    }

    //Save time spent
    t = clock() - t;

    if(count >= ms) {
        std::cout << "Success! Count: " << count << std::endl;

        // If succesful run: set ga metrics
        ga.averageGenerationCountOfLastSucces = generations/it;
        ga.averageNumberOfFitFnEvaluations = fFEvals/it;
        ga.averageRuntimeOfLastSucces = (((float)t)/CLOCKS_PER_SEC)/it;
    }
    else{
        std::cout << "Failed! Count:" << count << std::endl;
        std::cout << "Best fitness found was: " << bestFound << std::endl;
    }

    return(count >= ms);
}

void runExperiment(GeneticAlgorithm &ga, CrossoverOperator crossoverOperator, FitnessFunction fitnessFunction)
{
    const int INITIAL_POPULATION_SIZE = 10;
    const int MAXIMUM_POPULATION_SIZE = 1280;
    const int NUM_TRAINING_ITER = 25;
    const int MIN_TRAINING_SUCC = 24;
    int populationSize = INITIAL_POPULATION_SIZE;
    float maxFitness = 100;

    bool res;

    do
    {
        res = doTraining(ga, crossoverOperator, fitnessFunction, populationSize, NUM_TRAINING_ITER, MIN_TRAINING_SUCC, maxFitness);

        if (!res)
        {
            populationSize*=2;
        }
    }
    while(!res && populationSize*2 <= MAXIMUM_POPULATION_SIZE);

    std::cout << "Starting bisection search" << std::endl;

    if(populationSize*2 > MAXIMUM_POPULATION_SIZE){std::cout<<std::endl<<"Finished training"<<std::endl;}
    else{
        //Perform bisection search to find minimal value of populationSize
        int upper =  populationSize;        //This value returns true
        int lower = populationSize/2;       //This value returned false
        do{
            int populationSize = (lower+upper)/2;
            res = doTraining(ga, crossoverOperator, fitnessFunction, populationSize, NUM_TRAINING_ITER, MIN_TRAINING_SUCC, maxFitness);
            if(res){
                //Succesful run results in lowering the upper bound
                upper = populationSize;
            }
            else{
                //Unsuccesful run results in increasing lower bound
                lower = populationSize;
            }
        }
        while(upper - lower > 10 );
        // log upper
        std::cout<<"upper: "<<upper<<std::endl;
        // log metrics
        std::cout<<"avgGenCount: "<< ga.averageGenerationCountOfLastSucces
                 <<" avgRuntime: "<< ga.averageRuntimeOfLastSucces
                 <<" avgFFevals: "<< ga.numberOfFitFnEvalualtions
                 <<std::endl      <<std::endl;

    }
}

void printResults(int fitness, int iter, int cx, int ff, int ps){
    std::cout   <<"fitness;"    <<fitness
                <<";iteration;" <<iter
                <<";xOP;"      <<cx
                <<";fFun;"      <<ff
                <<";popSize;"   <<ps
                <<std::endl     <<std::endl;
}