#include <stdlib.h>
#include <stdio.h>
#include "individual.h"

//


struct Individual_t {
	int length;
	int nbVal;
	int *genoType;
	double quality;
	double qualityCumulation;
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
///:il est just demande de faire une seule permutaion
	int p = rand()%nbVal;
	int q = rand()%nbVal;
        int tmp = ind->genoType[p];

        ind->genoType[p] = ind->genoType[randval];
        ind->genoType[q] = tmp;



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


//!!!!fprintf
void individualPrint(FILE *fp, Individual *ind) {


	for (int i = 0; i < ind->length-1; ++i)
	{
		fprintf(fp, "%d, ", ind->genoType[i]);
	}
	fprintf(fp, "%d", ind->genoType[i]);

}


void individualSeqMutation(Individual *ind, float pm) {
	if (( (double)rand()/(double)RAND_MAX ) <= pm)
		individualRandomPermInit(Individual *ind);

	
}


Individual* individualSeqCrossOver(Individual *parent1, Individual *parent2) {
	
	genoType* genotype1 = parent1->genoType;
	genoType* genotype2 = parent2->genoType;
	int length = parent1->length;
	
	Individual* enfant = malloc(sizeof(Individual));
	genoType* geno_enfant = enfant->genoType;
	
	int i;
	int p = rand()%length;
	
	for( i = 0; i<p ; i++)
		geno_enfant[i] = genotype1[i];
	
	for( i = p; i<length; i++)
		geno_enfant[i] = genotype2[i];
	
	enfant->genoType = geno_enfant;
	
	return enfant;	


}


void individualPermMutation(Individual *ind, float pm) {
	
	genoType* genotype = ind->genoType;
	int length = ind->length;
	
	for(;;;)
	{
		int p1 = rand()%length;
		int p2 = rand()%length;
		
		if(p1!=p2)
			break;
	}
	
	if( p1>p2)
	{
		int a = p1;
		p1 = p2;
		p2 = a;
	}
	
	for(;p1>=p2;)
	{
		int a = genotype[p1];
		genotype[p1] = genotype[p2];
		genotype[p2] = a;
		
		p1++;
		p2--;
	}
	
	ind->genoType = genotype;
	


}


Individual* individualPermCrossOver(Individual *parent1, Individual *parent2) {


}
