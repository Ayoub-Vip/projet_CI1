#include <stdlib.h>
#include <stdio.h>
#include "individual.h"
#include "population.c"

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
	for (int i = 0; i < individualGetLength(ind); ++i)
		individualSetGene(ind, i, rand()%ind->nbVal);
		// ind->genoType[i] = rand()%ind->nbVal;		
}


void individualRandomPermInit(Individual *ind) {
///:il est just demande de faire une seule permutaion
	int p = rand()%ind->nbVal;
	int q = rand()%ind->nbVal;
        // int tmp = ind->genoType[p];
        int tmp = individualGetGene(ind, p);

        individualSetGene(ind,  p, individualGetGene(ind, q));
        individualSetGene(ind,  q, tmp);



        // ind->genoType[p] = ind->genoType[randval];
        // ind->genoType[q] = tmp;



}


Individual *individualCopy(Individual *ind) {

	Individual *copyIndividual;
	copyIndividual = malloc(sizeof(Individual));
	copyIndividual->genoType = calloc(ind->length, sizeof(int));
	
	for (int i = 0; i < ind->length; ++i)
		individualSetGene(copyIndividual, i, individualGetGene(ind, int i));
		// copyIndividual->genoType[i] = ind->genoType[i];


	copyIndividual->length = individualGetLength(ind);
	copyIndividual->nbVal = ind->nbVal;
	copyIndividual->quality = ind->quality;
	copyIndividual->qualityCumulation = ind->qualityCumulation;



	

	return copyIndividual;

}


//!!!!fprintf
void individualPrint(FILE *fp, Individual *ind) {


	for (int i = 0; i < individualGetLength(ind)-1; ++i)
	{
		fprintf(fp, "%d, ", individualGetGene(ind, i));
	}
	fprintf(fp, "%d", individualGetGene(ind, i));

}


void individualSeqMutation(Individual *ind, float pm) {
	if (( (double)rand()/(double)RAND_MAX ) <= pm)
		individualRandomPermInit(Individual *ind);

	
}


Individual* individualSeqCrossOver(Individual *parent1, Individual *parent2) {
	
/*	genoType* genotype1 = parent1->genoType;
	genoType* genotype2 = parent2->genoType;
	
	Individual* enfant = malloc(sizeof(Individual));
	enfant->length = individualGetLength(parent1);
	enfant->nbVal = parent1->nbVal;

	genoType* geno_enfant = enfant->genoType;
	int i;
	*/
	int p = rand()%length;
	Indivdual enfant = copyIndividual(parent1);
/*	
	for( i = 0; i<p ; i++)
		geno_enfant[i] = genotype1[i];
	*/
	for(int i = p; i<length; i++)
		individualSetGene(enfant,  i, ndividualGetGene(parent2, i))
		// geno_enfant[i] = genotype2[i];
	
	// enfant->genoType = geno_enfant;
	
	return enfant;	


}


void individualPermMutation(Individual *ind, float pm) {
	
	if (!(( (double)rand()/(double)RAND_MAX ) <= pm))
		return;

	int p1, p2;
	int length = ind->length;
	
	for(;;)
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
		int a = individualGetGene(ind, p1);
		ind->genoType[p1] = individualGetGene(ind, p2);
		// ind->genoType[p1] = ind->genoType[p2];
		individualSetGene(ind,  i, a);
		// ind->genoType[p2] = a;
		p1++;
		p2--;
	}
	
	
	}

Individual* individualPermCrossOver(Individual *parent1, Individual *parent2) {
	
	int p1 = rand()%individualGetLength(parent1);
	int p2 = rand()%individualGetLength(parent2);

		
	if( p1>p2){
		int a = p1;
		p1 = p2;
		p2 = a;
	}

	int tab[p2-p1+1];


	for (int i = p1; i <= p2; ++i)
	{
		
	}

	for (; p1 <= p2 ;){

	}

	int tabIndex[parent1->length+1]; //hypothese: length = nbVal 

	for (int i = 0; i < parent1->length; ++i) {
		tabIndex[ parent1->genoType[i]] = i;
	}





	for (; p1 <= p2 ;p1++)
	{
		if (parent1->genoType[p1] != parent2->genoType[p1] )
		{
			int a = individualGetGene(parent1, p1);
			int b = individualGetGene(parent2, p1);

			// parent1->genoType[tabIndex[b]] = a;
			individualSetGene(parent1,  tabIndex[b], a);
			// parent1->genoType[p1] = b ;
			individualSetGene(parent1,  p1, b);


			int tmp = tabIndex[b];
			tabIndex[b] = tabIndex[a];
			tabIndex[a] = tmp;
		

		}
		


	}



}



/*


	for (; p1 <= p2 ;)
	{
		if (parent1->genoType[p1] != parent2->genoType[p1] )
		{
			int a = parent1->genoType[p1];
			int b = parent2->genoType[p1];

			
			for (int i = 0; i < parent1->length; ++i)
			{
				if (parent1->genoType[i] == b)
				{
					parent1->genoType[i]  = a;
					parent1->genoType[p1] = b ;
					break;
				}
			}

		}
		p1--;


	}

*/
