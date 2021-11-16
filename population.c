#include <stdlib.h>
#include <stdio.h>
#include "population.h"
// #include "individual.c"

double individualGetQuality(Individual *ind);
double individualGetQualityCumulation(Individual *ind);
int *individualGetGenotype(Individual *ind);
void individualPutQuality(Individual *ind, double value);
void individualPutQualityCumulation(Individual *ind, double value);
Individual *individualCopy(Individual *ind);

// int compare ( const void *pa, const void *pb );

Individual *getpopind(Population *pop, int i);


struct Population_t {
	int eliteSize;
	int length;
	int nbVal;
	int size;
	float pmutation;
	double (*fitness)(Individual *, void *);
	double sumFitness;
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
	popInit->fitness = fitness;
	popInit->mutation = mutation;
	popInit->length = length;
	popInit->nbVal = nbVal;
	popInit->size = size;
	popInit->init = init;
	popInit->sumFitness = 0; //initialement

	if (init == NULL)
		return popInit;



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



	for( int i = 1; i < popInit->size; i++){

		QC += individualGetQuality(ind[i])/(popInit->sumFitness);

		individualPutQualityCumulation(ind[i], QC);

	}

	return popInit;

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
Individual *getpopind(Population *pop, int i){
	return pop->tableInd[i];
}

Individual *populationGetBestIndividual(Population *pop) {
	double maxFitness = populationGetMaxFitness(pop);

	for (int i = 0; i < pop->size; ++i)
		if(individualGetQuality(pop->tableInd[i]) == maxFitness)
			return pop->tableInd[i];

	return NULL;

}


void populationFree(Population *pop) {

	for (int i = 0; i < pop->size; ++i)
		free(pop->tableInd[i]);

	free(pop->tableInd);
	// free(pop->crossover);
	// free(pop->mutation);
	free(pop->paramFitness);
	// free(pop->fitness);
	// free(pop->init);

	free(pop);

}


//O(logn)
Individual *populationSelection(Population *pop) {

	double r = (double) rand()/(double) RAND_MAX;

	int lo = 0;
	int hi = pop->size-1;
	
	if (r == 1)
		return pop->tableInd[pop->size-1];

	while (lo < hi) {

		int mid = lo + (hi - lo) / 2;
		if (r < individualGetQualityCumulation(pop->tableInd[mid])) {

			if (mid == 0){
				return (pop->tableInd[mid]);
			}
			if (r >= individualGetQualityCumulation(pop->tableInd[mid-1]))
				return (pop->tableInd[mid]);
			else
				hi = mid - 1;
		}
		else {

			// if (mid == pop->size-1){
			// 	return (pop->tableInd[mid]);
			// }

			if (r < individualGetQualityCumulation(pop->tableInd[mid+1]))
				return (pop->tableInd[mid+1]);
			else
				lo = mid + 1;
		}
		// else {
		// 	return (pop->tableInd[mid]);
		// }d
		 // if (r >= individualGetQualityCumulation(pop->tableInd[mid]))
	}

	return NULL;

}

static int compare ( const void *pa, const void *pb ) {
	const double *a = *(const double **)pa;
	const double *b = *(const double **)pb;
	if(a[0] == b[0])
		return (int) (a[1] - b[1]);
	else
		return (int) (b[0] - a[0]);

	}





void populationEvolve(Population *pop) {
	srand(time(NULL));
	int eliteSize = pop->eliteSize;
	int size = pop->size;
	Individual* (*crossover_t)(Individual *, Individual *) = pop->crossover;
	double (*fitness_t)(Individual *, void *) = pop->fitness;
	void (*mutation_t)(Individual *, float) = pop->mutation;


	Individual** tableInd = pop->tableInd;
	double **table_fitness = malloc(size * sizeof(double*));

	for ( int i = 0; i < size; i++ ) {
		table_fitness[i] = malloc(2 * sizeof(double));
    	table_fitness[i][0] = individualGetQuality(tableInd[i]);
    	table_fitness[i][1] = i;

    }
	// fprintf(stderr, "operation 2 OK\n" );

    qsort(table_fitness, size, sizeof table_fitness[0], compare);
	// fprintf(stderr, "operation 3 OK\n" );


	Individual **nextGeneration;
	nextGeneration = calloc(size, sizeof(Individual*));
	// fprintf(stderr, "operation 4 OK\n" );

	//conservation de k meilleur ind
	for(int i = 0; i < eliteSize; i++)
	{
		int indice = (int) table_fitness[i][1];

		nextGeneration[i] = individualCopy(pop->tableInd[indice]);
		
		
	}

	for(int i = eliteSize; i < size; i++)
	{
		Individual *parent1, *parent2;
		
		do{
			parent1 = populationSelection(pop);
			parent2 = populationSelection(pop);
		
		} while (parent1 == parent2);

		nextGeneration[i] = crossover_t(parent1, parent2);
	}
	// fprintf(stderr, "operation 4 OK\n" );
	

	for(int i = 0; i < size; i++){
		individualFree(pop->tableInd[i]);
		pop->tableInd[i] = nextGeneration[i];

	}
	// fprintf(stderr, "operation 5 OK\n" );
	

	pop->sumFitness = individualGetQuality(tableInd[0]);
	individualPutQuality(pop->tableInd[0], fitness_t(pop->tableInd[0], pop->paramFitness));

	// fprintf(stderr, "operation 6 OK\n" );

	for (int i = 1; i < size; i++)
	{
		mutation_t(pop->tableInd[i], pop->pmutation);

		individualPutQuality(pop->tableInd[i], fitness_t(pop->tableInd[i], pop->paramFitness));

		pop->sumFitness += individualGetQuality(pop->tableInd[i]);

	}
	// fprintf(stderr, "operation 7 OK\n" );

	double QC = individualGetQuality(pop->tableInd[0])/(pop->sumFitness);
	individualPutQualityCumulation(pop->tableInd[0], QC);

	for(int i = 1; i < size; i++){

		QC += individualGetQuality(pop->tableInd[i])/(pop->sumFitness);

		individualPutQualityCumulation(pop->tableInd[i], QC);
	}

	// fprintf(stderr, "last operation  OK\n" );

	for (int i = 0; i < size; ++i)
		free(table_fitness[i]);



	free(table_fitness);
	

}

