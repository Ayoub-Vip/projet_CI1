#include <stdlib.h>
#include <stdio.h>
#include "individual.h"


//

double individualGetQuality(Individual *ind);
double individualGetQualityCumulation(Individual *ind);
int *individualGetGenotype(Individual *ind);
void individualPutQuality(Individual *ind, double value);
void individualPutQualityCumulation(Individual *ind, double value);

struct Individual_t {
	int length;
	int nbVal;
	int *genoType;
	double quality;
	double qualityCumulation;
};

int *individualGetGenotype(Individual *ind) {
	return ind->genoType;
}

double individualGetQuality(Individual *ind){
	return ind->quality;
}

double individualGetQualityCumulation(Individual *ind){
	return ind->qualityCumulation;
}

void individualPutQualityCumulation(Individual *ind, double value){
	ind->qualityCumulation = value;
}
void individualPutQuality(Individual *ind, double value){
	ind->quality = value;
}

Individual *individualCreate(int length, int nbVal) {

	Individual *newIndividual;
	newIndividual = malloc(sizeof(Individual));
	newIndividual->genoType = calloc(length, sizeof(int));
	newIndividual->length = length;
	newIndividual->nbVal = nbVal;


	return newIndividual;
}


void individualFree(Individual *ind) {
	free(ind->genoType);
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
	for (int i = 0; i < individualGetLength(ind); ++i)
		individualSetGene(ind, i, rand()%ind->nbVal);
	
}

///////////////////////////////////////////////////////////////////////
void individualRandomPermInit(Individual *ind) {


	int length = individualGetLength(ind);

	for(int i = 0; i < length; i++)
		individualSetGene(ind, i, i);

	for (int i = 0; i < length; i++) {

        int randval = rand()%(length-i) + i;
        int tmp = individualGetGene(ind, i);

        individualSetGene(ind, i, individualGetGene(ind, randval));
        individualSetGene(ind, randval, tmp);
 
    }
	

}


//////////////////////////////////////////////////////////////////
Individual *individualCopy(Individual *ind) {

	Individual *copyIndividual;
	int length = individualGetLength(ind);
	copyIndividual = malloc(sizeof(Individual));
	copyIndividual->genoType = calloc(length, sizeof(int));
	
	for (int i = 0; i < length; ++i)
		individualSetGene(copyIndividual, i, individualGetGene(ind, i));



	copyIndividual->length = length;
	copyIndividual->nbVal = ind->nbVal;
	copyIndividual->quality = ind->quality;
	copyIndividual->qualityCumulation = ind->qualityCumulation;

	return copyIndividual;

}


//!!!!fprintf
void individualPrint(FILE *fp, Individual *ind) {

	int i;
	for (i = 0; i < individualGetLength(ind)-1; ++i)
		fprintf(fp, "%d, ", individualGetGene(ind, i));
	
	fprintf(fp, "%d", individualGetGene(ind, i));

}


void individualSeqMutation(Individual *ind, float pm) {

	if (( (double)rand()/(double)RAND_MAX ) <= pm){

		int p, q;

		do{
			p = rand()%ind->nbVal;
			q = rand()%ind->nbVal;			
		}while(q == p);

		int a = individualGetGene(ind, p);
		individualSetGene(ind, p, individualGetGene(ind, q));
		individualSetGene(ind, q, a);

	}

	
}
///////////////////////////

Individual *individualSeqCrossOver(Individual *parent1, Individual *parent2) {


	Individual *enfant = individualCopy(parent1);
	int l = rand()%individualGetLength(parent1);

	for(int i = l; i< enfant->length; i++)
		individualSetGene(enfant,  i, individualGetGene(parent2, i));


	return enfant;	


}

/////////////////////////



void individualPermMutation(Individual *ind, float pm) {
	
	if ((( (double)rand()/(double)RAND_MAX ) <= pm))
	{
	int p1, p2;
	int length = ind->length;
	
	do{
		p1 = rand()%length;
		p2 = rand()%length;
	} while (p1 == p2);
	
	if(p1 > p2) {
		int a = p1;
		p1 = p2;
		p2 = a;
	}
	
	while(p1>=p2)
	{
		int a = individualGetGene(ind, p1);
		ind->genoType[p1] = individualGetGene(ind, p2);

		individualSetGene(ind,  p2, a);

		p1++;
		p2--;
	}
	}
	
	}

Individual *individualPermCrossOver(Individual *parent1, Individual *parent2)
{
	Individual *enfant = individualCopy(parent1);
	int p1 = rand()%individualGetLength(parent1);
	int p2 = rand()%individualGetLength(parent2);

		
	if(p1>p2){
		int a = p1;
		p1 = p2;
		p2 = a;
	}

	int tabIndex[enfant->length+1]; 

	for (int i = 0; i < enfant->length; ++i) {
		tabIndex[ enfant->genoType[i] ] = i;
	}





	for ( ; p1 <= p2; p1++)
	{
		
		int a = individualGetGene(enfant, p1);
		int b = individualGetGene(parent2, p1);


		individualSetGene(enfant,  tabIndex[b], a);
		individualSetGene(enfant,  p1, b);


		int tmp = tabIndex[b];
		tabIndex[b] = tabIndex[a];
		tabIndex[a] = tmp;


	}
	return enfant;

}

