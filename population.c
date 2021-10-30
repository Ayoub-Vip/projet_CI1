#include <stdlib.h>
#include <stdio.h>
#include "population.h"
#include "individual.c"


/*
int length, int nbVal, int size,
    double (*fitness)(Individual *, void *),
    void *paramFitness,
    void (*init)(Individual *),
    void (*mutation)(Individual *, float), float pmutation,
    Individual* (*crossover)(Individual *, Individual *),
    int eliteSize);

*/


struct Population_t {
	int length;
	int nbVal;
	int size;
        int eliteSize;
	int **genoTypes;
	float pmutation;
        double (*fitness)(Individual *, void *);
        void *paramFitness;
        void (*init)(Individual *);
        void (*mutation)(Individual *, float);
        Individual* (*crossover)(Individual *, Individual *);

};
Population *populationInit(int length, int nbVal, int size,
                           double (*fitness)(Individual *, void *),
			   void *paramFitness,
                           void (*init)(Individual *),
                           void (*mutation)(Individual *, float), float pmutation,
                           Individual* (*crossover)(Individual *, Individual *),
                           int eliteSize) 
{

	Population *populationInit;
	populationInit = mallloc(sizeof(Population));
	populationInit->genoTypes = calloc(populationInit->size, sizeof(Individual));
	populationInit->length = length;
	populationInit->nbVal = nbVal;
	populationInit->size = size;
	populationInit->paramFitness  = paramFitness;
	populationInit->eliteSize = eliteSize;
	populationInit->pmutation = pmutation;
	populationInit->init = init;
	populationInit->mutation = mutation;
	populationInit->crossover = crossover;

	for (int i = 0; i < sizeof; ++i)
	{
		populationInit->genoTypes[i] = init();
	}










}



double populationGetMaxFitness(Population *pop) {


}



double populationGetMinFitness(Population *pop) {


}



double populationGetAvgFitness(Population *pop) {


}


Individual *populationGetBestIndividual(Population *pop) {


}


void populationFree(Population *pop) {


}



Individual *populationSelection(Population *pop) {


}



void populationEvolve(Population *pop) {


}

