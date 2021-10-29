#include <stdlib.h>
#include <stdio.h>
#include "individual.h"

//


struct Individual_t {
	int length;
	int nbVal;
	int *genoType;
	// double quality;
};


Individual *individualCreate(int length, int nbVal) {

	Individual *newIndividual;
	newIndividual = malloc(sizeof(Individual));
	newIndividual->genoType = calloc(length, sizeof(int));
	newIndividual->length = length;
	newIndividual->nbVal = nbVal;

	

	return newIndividual;

}


void individualFree(Individual *ind) {
	free(ind->genoType)
	free(ind);
}


int individualGetLength(Individual *ind) {
	return ind->length;
	
}


int individualGetGene(Individual *ind, int i) {
	return ind->genoType;
	
}


int individualGetGene(Individual *ind, int i){
	return ind->genoType[i];

}


void individualSetGene(Individual *ind, int i, int val) {
	ind->genoType[i] = val;
}


void individualRandomInit(Individual *ind) {

	for (int i = 0; i < ind->length; ++i)
		ind->genoType[i] = rand()%ind->nbVal;
}


void individualRandomPermInit(Individual *ind) {
	int tmp_index1 = rand()%ind->length;
	int tmp_index2 = rand()%ind->length;

	int tmp_val1    = ind->genoType[tmp_index1];
	ind->genoType[tmp_index1]  = ind->genoType[tmp_index2];
	ind->genoType[tmp_index2]  = tmp_val1;


	ind->genoType[rand()%ind->length] = ind->genoType[rand()%ind->length];

}


Individual *individualCopy(Individual *ind) {

	Individual *copyIndividual;
	copyIndividual = malloc(sizeof(Individual));
	copyIndividual->genoType = calloc(ind->length, sizeof(int));
	
	for (int i = 0; i < ind->length; ++i)
		copyIndividual->genoType[i] = ind->genoType[i];

	copyIndividual->length = ind->length;
	copyIndividual->nbVal = ind->nbVal;

	

	return copyIndividual;

}



void individualPrint(FILE *fp, Individual *ind) {


	for (int i = 0; i < ind->length-1; ++i)
	{
		fprintf(fp, "%d, ", ind->genoType[i]);
	}
	fprintf(fp, "%d", ind->genoType[i]);

}


void individualSeqMutation(Individual *ind, float pm) {
	
}


Individual* individualSeqCrossOver(Individual *parent1, Individual *parent2) {


}


void individualPermMutation(Individual *ind, float pm) {


}


Individual* individualPermCrossOver(Individual *parent1, Individual *parent2) {


}
