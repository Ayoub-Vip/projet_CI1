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
	float pmutation;
	double sumFitness;
	double (*fitness)(Individual *, void *);
	void *paramFitness;
	void (*init)(Individual *);
	void (*mutation)(Individual *, float);
	Individual** tableInd;
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
	populationInit->tableInd = calloc(populationInit->size, sizeof(Individual));
	populationInit->length = length;
	populationInit->nbVal = nbVal;
	populationInit->size = size;
	populationInit->paramFitness  = paramFitness;	
	populationInit->eliteSize = eliteSize;
	populationInit->pmutation = pmutation;
	populationInit->init = init;
	populationInit->mutation = mutation;
	populationInit->crossover = crossover;
	populationInit->sumFitness = 0;

	for (int i = 0; i < populationInit->size; ++i)
		if(populationInit->tableInd[i] = individualCreate(length, nbVal))
			{ 

				init(populationInit->tableInd[i]);
				(populationInit->tableInd[i])->quality = pop->fitness(pop->tableInd[i], pop->paramFitness);
				populationInit->sumFitness += (populationInit->tableInd[i])->quality;
			}
	
	population->tableInd[0]->qualityCumulation = (population->tableInd[0]->quality)/(population->sumFitness);
	
	for( int i = 0; i<populationInit->size-1; i++)
		
		populationInit->tableInd[i+1]->qualityCumulation = populationInit->tableInd[i]->qualityCumulation + ((populationInit->tableInd[i+1]->quality)/populationInit->sumFitness);
	


}



double populationGetMaxFitness(Population *pop) {
	double max;
	for (int i = 0; i < pop->size-1; ++i)
	{
		double a =pop->fitness(pop->tableInd[i], pop->paramFitness);
		double b =pop->fitness(pop->tableInd[i+1], pop->paramFitness);

		max = (a > b) ? a : b;
	}

	return max;
}



double populationGetMinFitness(Population *pop) {
	double min = pop->fitness(pop->tableInd[0], pop->paramFitness);

	for (int i = 1; i < pop->size-1; ++i)
	{
		double a =pop->fitness(pop->tableInd[i], pop->paramFitness);
		double b =pop->fitness(pop->tableInd[i+1], pop->paramFitness);

		min = (a < b) ? a : b;
	}

	return min;

}



double populationGetAvgFitness(Population *pop) {

	return (pop->sumFitness/(double) pop->size);

}


Individual *populationGetBestIndividual(Population *pop) {
	double maxFitness = populationGetMaxFitness(pop);

	for (int i = 0; i < pop->size; ++i)
	{
		if(pop->fitness(pop->tableInd[i], pop->paramFitness) == maxFitness)
			return pop->tableInd[i];
	}

}


void populationFree(Population *pop) {

	for (int i = 0; i < pop->size; ++i)
		free(pop->tableInd[i]);

	free(pop->tableInd);
	free(pop);

}


//O(logn)

Individual *populationSelection(Population *pop) {

	r = (double) rand()/(double) RAND_MAX;

	double sum = 0;
	int lo = 0;
	int hi = pop->size-1;
	
	while ( lo < hi){
		int mid = lo + (hi - lo) / 2;
		if (r < (pop->tableInd[mid])->qualityCumulation)
		
			if (r > (pop->tableInd[mid-1])->qualityCumulation)
				return (pop->tableInd[mid]);
			else
				hi = mid - 1;
		
		else if (r > (pop->tableInd[mid])->qualityCumulation)

			if (r < (pop->tableInd[mid+1])->qualityCumulation)
				return (pop->tableInd[mid+1]);
			else
				lo = mid + 1;

		else
			return (pop->tableInd[mid]);

	}
/*int binary_search_aux(int key, int tab[], int lo, int hi) {
int lo = 0;
int hi = n-1;
  while (lo <= hi) {
    int mid = lo + (hi - lo) / 2;
    if (key < tab[mid])
      hi = mid - 1;
    else if (key > tab[mid])
      lo = mid + 1;
    else
      return mid;
}}*/


}



void populationEvolve(Population *pop) {
	
	int eliteSize = pop->eliteSize;
	int size = pop->size;
	individual** tableInd = pop->tableInd;
	void* paramFitness = pop->paramFitness;
	double* fitness = pop->fitness;
	
	double table_fitness[size];
	int elite_individus[eliteSize-1];
	int k = 0;
	
	for (int i = 0; i < size; i++)
	{
		table_fitness[i] = tableInd[i]->quality;
	}
	
	mergesort(table_fitness, size);
	
	for( int i = 0; i<size && k<eliteSize-1; i++)
	{
		
		if( tableInd[i]->quality >=table_fitness[size - eliteSize] && tableInd[i]->quality <= table_fitness[size-2])
		{
			elite_individus[k] = i;
			k++;
		}
	}
	
	population* pop_t = populationInit(pop->length, pop->nbVal, size, fitness, paramFitness,pop->init,pop->mutation,pop->crossover,eliteSize);
	
	pop_t->tableInd[0] = populationGetBestIndividual();
	
	for(int i = 0; i<=k; i++)
	{
		pop_t->tableInd[i+1] = tableInd[elite_individus[i]];
	}
	
	for(int i = k+1; i<size; i++)
	{
		individual* parent1 = populationSelection(pop);
		individual* parent2 = populationSelection(pop); /* faudrait vérifier que les parents sont différents*/
		
		pop_t->tableInd[i] = individualSeqCrossOver(parent1, parent2);
	}
	
	for( int i = 1, pop_t->sumFitness = tableInd[0]->quality ; i<size; i++)
	{
		individualSeqMutation(pop_t->tableInd[i], pop->pmutation);
		pop_t->tableInd[i]->quality = fitness(pop_t->tableInd[i], paramFitness);
		pop_t->sumFitness = pop_t->sumFitness + pop_t->tableInd[i]->quality;

	}
	
	pop_t->tableInd[0]->qualityCumulation = (pop_t->tableInd[0]->quality)/(pop_t->sumFitness);
	
	for( i = 0; i<size-1; i++)
	{
		 pop_t->tableInd[i+1]->qualityCumulation = pop_t->tableInd[i]->qualityCumulation + ((pop_t->tableInd[i+1]->quality)/pop_t->sumFitness);
	}
	
	population* p = pop;
	pop = pop_t;
	
	populationFree(p);
		
 

}

