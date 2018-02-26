#include "GeneticAlgorithm.h"
#include <iostream>
#include <time.h>

void printResults(int fness, int iter, int cx, int ff, int ps);
bool doTraining(GeneticAlgorithm &ga, CrossoverOperator cx, FitnessFunction ff, int ps, int is, int ms, int mf);

int main() {
    const int INITIAL_POPULATION_SIZE = 10;
    const int MAXIMUM_POPULATION_SIZE = 1280;
    const int NUM_TRAINING_ITER = 25;
    const int MIN_TRAINING_SUCC = 24;

    std::cout << "Starting Program" << std::endl << std::endl;

    // Create instance of GA
    GeneticAlgorithm ga;

    CrossoverOperator crossoverOperator = CrossoverOperator::uniformX;
    FitnessFunction fitnessFunction = FitnessFunction::countingOnes;
    int populationSize = INITIAL_POPULATION_SIZE;
    int maxFitness = 100;

    /* TODO: Repeat each experiment 25x for each population size
       TODO: and repeat that with population size change rules
       TODO: until 24/25 runs return optimal solution*/

    std::cout << "Starting Experiment 1" << std::endl << std::endl;

    bool res;

    // Experiment 1
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
        std::cout<<"avgGenCount: "<<ga.averageGenerationCountOfLastSucces<<" avgRuntime: "<< ga.averageRuntimeOfLastSucces <<std::endl;

    }

    // TODO: Experiments 2 - 5

    return 0;
}

bool doTraining(GeneticAlgorithm &ga, CrossoverOperator cx, FitnessFunction ff, int ps, int it, int ms, int mf){
    std::cout << "Trying with population size = " << ps << ": ";
    int count = 0; int generations = 0;

    //Init clock
    clock_t t = clock();

    // Rep 25 times
    for(int n = 0; n < it; n++){
        //Run the training step
        int bestSolutionFitness = ga.run(cx, ff, ps);

        //Add to number of generations
        generations += ga.generationCount;

        std::cout<<"generations: "<<ga.generationCount<<", popSize: "<<ps<<", FFnEvals: "<<ga.numberOfFitFnEvalualtions<<std::endl;

        //Add to number of successes
        if(bestSolutionFitness == mf){count++;}
    }

    //Save time spent
    t = clock() - t;

    if(count >= ms) {
        std::cout << "Success! Count: " << count << std::endl;

        // If succesful run: set ga.averageTimeOfLastSuccesfullRun
        ga.averageGenerationCountOfLastSucces = generations/it;

        // If succesful run: set ga.avergaGenerationCountOfLastSuccesfullRun
        ga.averageRuntimeOfLastSucces = (((float)t)/CLOCKS_PER_SEC)/it;
    }
    else{
        std::cout << "Failed! Count:" << count << std::endl;
    }

    return(count >= ms);
}

void printResults(int fitness, int iter, int cx, int ff, int ps){
    std::cout   <<"fitness;"    <<fitness
                <<";iteration;" <<iter
                <<";xOP;"      <<cx
                <<";fFun;"      <<ff
                <<";popSize;"   <<ps
                <<std::endl;
}