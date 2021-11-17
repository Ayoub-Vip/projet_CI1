#include <stdlib.h>
#include <stdio.h>
#include "population.h"

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
	double* quality;
	double* qualitycumulation;

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
	popInit->quality = malloc(size*sizeof(double));
	popInit->qualitycumulation = malloc(size*sizeof(double));
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
	popInit->sumFitness = 0;

	if (init == NULL)
		return popInit;


	Individual **ind = popInit->tableInd;

	for (int i = 0; i < popInit->size; ++i)
	{
		if((ind[i] = individualCreate(length, nbVal)) != NULL)
		{
			init(ind[i]);
			popInit->quality[i] = fitness(ind[i],paramFitness);
			popInit->sumFitness += popInit->quality[i];
		}
	}



	double QC = popInit->quality[0]/(popInit->sumFitness);
	popInit->qualitycumulation[0] = QC;


	for( int i = 1; i < popInit->size; i++){

		QC += popInit->quality[i]/popInit->sumFitness;
		popInit->qualitycumulation[i] = QC;

	}

	return popInit;

}



double populationGetMaxFitness(Population *pop) {

	double max = pop->quality[0];

	for (int i = 1; i < pop->size; ++i)
	{
		double q = pop->quality[i];

		max = (q > max) ? q : max;
	}

	return max;

}



double populationGetMinFitness(Population *pop) {

	double min = pop->quality[0];

	for (int i = 1; i < pop->size; ++i)
	{
		double q = pop->quality[i];

		min = (q < min) ? q : min;
	}

	return min;

}



double populationGetAvgFitness(Population *pop) {

	return (pop->sumFitness/(double) pop->size);

}

Individual *populationGetBestIndividual(Population *pop) {
	double maxi = pop->quality[0];
	int indice;
	for (int i = 0; i < pop->size; ++i)
	{
		double q = pop->quality[i];
		if(q >= maxi){
			maxi = q;
			indice = i;
		}

	}


	// for (int i = 0; i < pop->size; ++i)
	// 	if(pop->quality[i] == maxFitness)
	// 		return pop->tableInd[i];

	return pop->tableInd[indice];

}


void populationFree(Population *pop) {

	for (int i = 0; i < pop->size; ++i)
		individualFree(pop->tableInd[i]);

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
		if (r < pop->qualitycumulation[mid]) {

			if (mid == 0){
				return (pop->tableInd[mid]);
			}
			if (r >= pop->qualitycumulation[mid-1])
				return (pop->tableInd[mid]);
			else
				hi = mid - 1;
		}
		else {

			// if (mid == pop->size-1){
			// 	return (pop->tableInd[mid]);
			// }

			if (r < pop->qualitycumulation[mid+1])
				return (pop->tableInd[mid+1]);
			else
				lo = mid + 1;
		}
		
	}

	return NULL;

}



// static int compare ( const void *pa, const void *pb ) {
// 	const double *a = *(const double **)pa;
// 	const double *b = *(const double **)pb;
// 	if(a[0] == b[0])
// 		return  (a[1] - b[1]);
// 	else
// 		return  (1073741824*b[0] - 1073741824*a[0]);

// 	}



static int compare ( const void *pa, const void *pb ) {
	const double *a = *(const double **)pa;
	const double *b = *(const double **)pb;
	if(a[0] < b[0])
		return  1;
	else if (a[0] > b[0])
		return -1;
	else
		return 0;
	

	}


void populationEvolve(Population *pop) {
	
	int eliteSize = pop->eliteSize;
	int size = pop->size;
	double **table_fitness = calloc(size, sizeof(double*));
	Individual **nextGeneration = calloc(size, sizeof(Individual*));


	for ( int i = 0; i < size; i++ ) {
		table_fitness[i] = malloc(2 * sizeof(double));
    	table_fitness[i][0] = pop->quality[i];
    	table_fitness[i][1] = i;

    }

    // for (int i = 0; i < size; ++i)
    // {
    // 	// int indice = (int) table_fitness[i][1];

    // 	fprintf(stderr,"\nvaleur fitness avant%f\n", table_fitness[i][0]);

    	individualPrint(stderr, pop->tableInd[0]);
    // }

    // qsort(table_fitness, size, sizeof table_fitness[0], compare);

//     for (int i = 0; i < size; ++i)
//     {
//     	fprintf(stderr,"\nvaleur fitness apres%f\n", table_fitness[i][0]);
//     	int indice = (int) table_fitness[i][1];
//     	individualPrint(stderr, pop->tableInd[indice]);
//     }
// printf("\nfn\n");
	//Conservation de k meilleur ind
	for(int i = 0; i < eliteSize; i++)
	{
		int indice = (int) table_fitness[i][1];

		nextGeneration[i] = individualCopy(pop->tableInd[indice]);
		
		
	}

	//Selection des parents
	for(int i = eliteSize; i < size; i++)
	{
		Individual *parent1, *parent2;
		
		do{
			parent1 = populationSelection(pop);
			parent2 = populationSelection(pop);
		
		} while (parent1 == parent2);

		nextGeneration[i] = pop->crossover(parent1, parent2);
	}


	for(int i = 0; i < size; i++){
		individualFree(pop->tableInd[i]);
		pop->tableInd[i] = nextGeneration[i];

	}

	pop->quality[0] = pop->fitness(pop->tableInd[0], pop->paramFitness);
	pop->sumFitness = pop->quality[0];

	for (int i = 1; i < size; i++)
	{
		pop->mutation(pop->tableInd[i], pop->pmutation);
		pop->quality[i] = pop->fitness(pop->tableInd[i], pop->paramFitness);
		pop->sumFitness += pop->quality[i];

	}

	double QC = pop->quality[0]/(pop->sumFitness);

	pop->qualitycumulation[0] = QC;


	for(int i = 1; i < size; i++){

		QC += pop->quality[i]/(pop->sumFitness);
		pop->qualitycumulation[i] = QC;

	}


	for (int i = 0; i < size; ++i)
		free(table_fitness[i]);

	free(table_fitness);
	

}



// void populationEvolve(Population *pop) {
// 	fprintf(stderr, "operation 1 OK\n" );

// 	int eliteSize = pop->eliteSize;
// 	int size = pop->size;
// 	Individual* (*crossover_t)(Individual *, Individual *) = pop->crossover;
// 	double (*fitness_t)(Individual *, void *) = pop->fitness;
// 	void (*mutation_t)(Individual *, float) = pop->mutation;


//     Population *pop_t =  populationInit(pop->length, pop->nbVal, pop->size,
//                            fitness_t,
//                            pop->paramFitness,
//                            NULL,
//                            pop->mutation,
//                            pop->pmutation,
//                            pop->crossover,
//                            eliteSize);

// 	fprintf(stderr, "operation 7 OK\n" );

// 	double **table_fitness = malloc(size * sizeof(double*));

// 	for ( int i = 0; i < size; i++ ) {
// 		table_fitness[i] = malloc(2 * sizeof(double));

//     	table_fitness[i][0] = pop->quality[i];
//     	table_fitness[i][1] = i;

//     }
//     qsort(table_fitness, size, sizeof table_fitness[0], compare);

// 	fprintf(stderr, "operation 7 OK\n" );
    
//     for(int i = 0; i < eliteSize; i++)
// 	{
// 		int indice = (int) table_fitness[i][1];

// 		pop_t->tableInd[i] = individualCopy(pop->tableInd[indice]);
		
		
// 	}

// 	for(int i = eliteSize; i < size; i++)
// 	{
// 		Individual *parent1, *parent2;
		
// 		do{
// 			parent1 = populationSelection(pop);
// 			parent2 = populationSelection(pop);
		
// 		} while (parent1 == parent2);

// 		pop_t->tableInd[i] = crossover_t(parent1, parent2);
// 	}

// 	fprintf(stderr, "operation 7 OK\n");


// 	for (int i = 1; i < size; i++)
// 	{
// 		mutation_t(pop_t->tableInd[i], pop->pmutation);
// 	}


	


// 	// individualPutQuality(pop->tableInd[0], fitness_t(pop->tableInd[0], pop->paramFitness));




// double sumf=0;
// 	for (int i = 0; i < size; ++i)
// 	{
// 		sumf += fitness_t(pop_t->tableInd[i], pop->paramFitness);
// 	}

// 	pop_t->sumFitness = sumf;

// 	for (int i = 0; i < size; ++i)
// 	{
// 		pop_t->quality[i]= fitness_t(pop_t->tableInd[i], pop->paramFitness);
// 	}
// 	pop_t->qualitycumulation[0]= fitness_t(pop_t->tableInd[0], pop->paramFitness)/pop_t->sumFitness;

// 	for (int i = 1; i < size; ++i)
// 	{
// 		pop_t->qualitycumulation[i]= pop_t->qualitycumulation[i-1] + fitness_t(pop_t->tableInd[i], pop->paramFitness)/pop_t->sumFitness;
// 	}
// 	fprintf(stderr, "operation 7 OK\n" );


// // Population *tmp  = pop;
// populationFree(tmp);
// pop = pop_t;

// 	fprintf(stderr, "operation 70 OK\n" );

// 	for (int i = 0; i < size; ++i)
// 		free(table_fitness[i]);



// 	free(table_fitness);
	
// 	printf( "operation 70 OK\n" );






// }
