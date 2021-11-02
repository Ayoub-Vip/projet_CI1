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

	genoType* genotype = ind->genoType;

	for (int i = 0; i < ind->length; i++)
	{
		genotype[i] = rand()%2;
	}
	
	ind->genoType = genotype;		
}


void individualRandomPermInit(Individual *ind) {
	
	genoType* genotype = ind->genotype;
	int length = ind->length;
	int nbVal = ind->nbVal;
	int i;
	
	for( i = 0; i<length; i++)
	{
		genotype[i] = i;
	}
	
	for( i = 0; i<length, i++)
	{
		int p = rand()%nbVal;
		int q = rand()%nbVal;
		
		if( p == q)
			continue;
		else
		{
			int a = genotype[p];
			genotype[p] = genotype[q];
			genotype[q] = a;
		}
	}
	
	ind->genoType = genotype;	

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
	
		genoType* genotype = ind->genoType;
	int length = ind->length;
	
	for(;;;)
	{
		int p = rand()%length;
		int q = rand()%length;
		
		if( p!=q)
			break;
	}
	
	int a = genotype[p];
	genotype[p] = genotype[q];
	genotype[q] = a;
	
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
