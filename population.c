#include <stdlib.h>
#include <stdio.h>
#include "population.h"
// #include "individual.c"



struct Population_t {
	int eliteSize;
	int length;
	int nbVal;
	int size;
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
                           void (*mutation)(Individual *, float),
                           float pmutation,
                           Individual* (*crossover)(Individual *, Individual *),
                           int eliteSize)
{

	Population *popInit;
	popInit = malloc(sizeof(Population));
	popInit->tableInd = calloc(size, sizeof(Individual *));
	popInit->paramFitness  = paramFitness;	
	popInit->eliteSize = eliteSize;
	popInit->pmutation = pmutation;
	popInit->crossover = crossover;
	popInit->mutation = mutation;
	popInit->length = length;
	popInit->nbVal = nbVal;
	popInit->size = size;
	popInit->init = init;
	popInit->sumFitness = 0;

	Individual **ind = popInit->tableInd;
	

	for (int i = 0; i < popInit->size; ++i)
	{
		if((ind[i] = individualCreate(length, nbVal)) != NULL)
		{ 
			init(ind[i]);
			individualPutQuality(ind[i], fitness(ind[i], paramFitness));
			popInit->sumFitness += individualGetQuality(ind[i]);
		}
	}

	double QC = individualGetQuality(ind[0])/(popInit->sumFitness);

	individualPutQualityCumulation(ind[0], QC);
	// ind[0]->qualityCumulation = (popInit->tableInd[0]->quality)/(popInit->sumFitness);


	for( int i = 1; i < popInit->size; i++){

		QC += individualGetQuality(ind[i])/(popInit->sumFitness);

		individualPutQualityCumulation(ind[i], QC);

		// popInit->tableInd[i+1]->qualityCumulation = popInit->tableInd[i]->qualityCumulation 
		// + ((popInit->tableInd[i+1]->quality)/popInit->sumFitness);
	}

}



double populationGetMaxFitness(Population *pop) {

	double max = individualGetQuality(pop->tableInd[0]);

	for (int i = 1; i < pop->size; ++i)
	{
		double q =individualGetQuality(pop->tableInd[i]);

		max = (q > max) ? q : max;
	}

	return max;

}



double populationGetMinFitness(Population *pop) {

	double min = individualGetQuality(pop->tableInd[0]);

	for (int i = 1; i < pop->size; ++i)
	{
		double q =individualGetQuality(pop->tableInd[i]);

		min = (q < min) ? q : min;
	}

	return min;

}



double populationGetAvgFitness(Population *pop) {

	return (pop->sumFitness/(double) pop->size);

}


Individual *populationGetBestIndividual(Population *pop) {
	double maxFitness = populationGetMaxFitness(pop);

	for (int i = 0; i < pop->size; ++i)
		if(individualGetQuality(pop->tableInd[i]) == maxFitness)
			return pop->tableInd[i];


}


void populationFree(Population *pop) {

	for (int i = 0; i < pop->size; ++i)
		free(pop->tableInd[i]);

	free(pop->tableInd);
	free(pop);

}


//O(logn)
Individual *populationSelection(Population *pop) {

	double r = (double) rand()/(double) RAND_MAX;

	int lo = 0;
	int hi = pop->size-1;
	
	while (lo < hi) {

		int mid = lo + (hi - lo) / 2;
		if (r < individualGetQualityCumulation(pop->tableInd[mid])) {
		
			if (r > individualGetQualityCumulation(pop->tableInd[mid-1]))
				return (pop->tableInd[mid]);
			else
				hi = mid - 1;
		}
		if (r > individualGetQualityCumulation(pop->tableInd[mid])) {

			if (r < individualGetQualityCumulation(pop->tableInd[mid+1]))
				return (pop->tableInd[mid+1]);
			else
				lo = mid + 1;
		}
		else {
			return (pop->tableInd[mid]);
		}
	}


}



void populationEvolve(Population *pop) {
	
	int eliteSize = pop->eliteSize;
	int size = pop->size;
	Individual** tableInd = pop->tableInd;
	void* paramFitness = pop->paramFitness;
	double (*fitness) = pop->fitness;
	
	double table_fitness[size];
	int elite_individus[eliteSize-1];
	int k = 0;
	
	for (int i = 0; i < size; i++)
		table_fitness[i] = individualGetQuality(tableInd[i]);
	
	
	mergesort(table_fitness, size);
	
	for( int i = 0; i<size && k<eliteSize-1; i++)
	{
		
		if( individualGetQuality(tableInd[i]) >=table_fitness[size - eliteSize] && individualGetQuality(tableInd[i]) <= table_fitness[size-2])
		{
			elite_individus[k] = i;
			k++;
		}
	}
	
	Population* pop_t = populationInit(pop->length, pop->nbVal, size, pop->fitness,
									   paramFitness,pop->init, pop->mutation,
									   pop->pmutation, pop->crossover, eliteSize);
	
	pop_t->tableInd[0]= populationGetBestIndividual(pop);
	
	for(int i = 0; i <= k; i++)
	{
		pop_t->tableInd[i+1] = tableInd[elite_individus[i]];
	}
	
	for(int i = k+1; i<size; i++)
	{
		Individual *parent1, *parent2;
		
		do{
			parent1 = populationSelection(pop);
			parent2 = populationSelection(pop); /* faudrait vérifier que les parents sont différents*/
		
		} while (parent1 == parent2);

		pop_t->tableInd[i] = individualSeqCrossOver(parent1, parent2);
	}
	
	pop_t->sumFitness = individualGetQuality(tableInd[0]);
	for(int i = 1; i < size; i++)
	{
		individualSeqMutation(pop_t->tableInd[i], pop->pmutation);
		// pop_t->tableInd[i]->quality = pop->fitness(pop_t->tableInd[i], paramFitness);
		individualPutQuality(pop_t->tableInd[i], pop->fitness(pop_t->tableInd[i], paramFitness));

		pop_t->sumFitness += individualGetQuality(pop_t->tableInd[i]);

	}
	
	// pop_t->tableInd[0]->qualityCumulation = (pop_t->tableInd[0]->quality)/(pop_t->sumFitness);



	double QC = individualGetQuality(pop_t->tableInd[0])/(pop_t->sumFitness);

	individualPutQualityCumulation(pop_t->tableInd[0], QC);


	for(int i = 1; i < pop_t->size; i++){

		QC += individualGetQuality(pop_t->tableInd[i])/(pop_t->sumFitness);

		individualPutQualityCumulation(pop_t->tableInd[i], QC);
	}
	////////////

	
	/*for(int i = 0; i<size-1; i++)
	{
		 pop_t->tableInd[i+1]->qualityCumulation = pop_t->tableInd[i]->qualityCumulation + 
		                                          ((pop_t->tableInd[i+1]->quality)/pop_t->sumFitness);
	}*/
	
	Population* p = pop;
	pop = pop_t;
	
	populationFree(p);
		
 
}

