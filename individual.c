#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "individual.h"

struct Individual_t {
	int length;
	int nbVal;
	int *genoType;

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
		individualSetGene(ind, i, rand() % ind->nbVal);
	
}


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


	return copyIndividual;

}

void individualPrint(FILE *fp, Individual *ind) {

	int i;
	for (i = 0; i < individualGetLength(ind)-1; ++i)
		fprintf(fp, "%d, ", individualGetGene(ind, i));
	
	fprintf(fp, "%d", individualGetGene(ind, i));

}


void individualSeqMutation(Individual *ind, float pm) {
	int length = individualGetLength(ind);
	for(int i = 0; i< length; i++){
	if (( (double)rand()/(double)RAND_MAX ) <= pm){
		ind->genoType[i] = rand()%ind->nbVal;

	}
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

	if ((( (double)rand()/(double)RAND_MAX ) <= pm)) {

		int length = ind->length;
		

		int p1 = rand()%length;
		int p2 = rand()%length;
		
		if(p1 > p2) {
			int a = p1;
			p1 = p2;
			p2 = a;
		}
		
		while( p1 < p2 )
		{
			int a = individualGetGene(ind, p1);
			ind->genoType[p1] = individualGetGene(ind, p2);
			individualSetGene(ind,  p1, individualGetGene(ind, p2));

	
			individualSetGene(ind,  p2, a);
	
			p1++;
			p2--;
		}
	}
	
}

Individual *individualPermCrossOver(Individual *parent1, Individual *parent2)
{
	Individual *enfant = individualCopy(parent1);
	int length = individualGetLength(parent1);

	int p1, p2;
	
	do{

		p2 = rand()%(length);
		p1 = rand()%(length);

	} while (p1 == p2);

	if(p1>p2){
		int a = p1;
		p1 = p2;
		p2 = a;
	}

	int tabIndex[enfant->length]; 

	for (int i = 0; i < enfant->length; ++i) {
		tabIndex[ parent1->genoType[i] ] = i;
	}

	for (int i1 = p1 ; i1 <= p2; i1++)
	{
		
		int a = individualGetGene(enfant, i1);
		int b = individualGetGene(parent2, i1);


		individualSetGene(enfant,  tabIndex[b], a);
		individualSetGene(enfant,  i1, b);


		int tmp = tabIndex[b];
		tabIndex[b] = tabIndex[a];
		tabIndex[a] = tmp;

	}
	return enfant;

}
