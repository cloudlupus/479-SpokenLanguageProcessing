//David Shagam
//vq_train
//assignment 2
//csci 479

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>
int K;
int D;
int N;
double** entries;
double** defaults;

//Computes the nearest centroied only holding onto the value most important.
int computeNearestCentroid(int ind){
	int nearestInd= 0;
	double lastcalc = DBL_MAX;
	double calc=0;
	int i;
	for(i = 0; i < K; i++){
		int j;
		for(j = 0; j < D; j++){
			double temp = (defaults[ind][j] - entries[i][j] );
			temp = temp*temp;
			calc = calc + temp;
		}
		if(calc < lastcalc){
			nearestInd = i;
			lastcalc = calc;
		}
		calc = 0;
	}
	return nearestInd;
}

//Calculates the average of the points that share a common index.
double* meanOfPoints(double **ent ,int *nearest, int ind){
	double *retVals = malloc( sizeof(double) * D);
	int j;
	for(j =0; j < D; j++){
		retVals[j] = entries[ind][j];
	}
	int numI=0;
	for(j = 0; j < N; j++){
		if(nearest[j] == ind){
			numI++;
			int i;
			for(i = 0; i< D; i++){
				retVals[i] += ent[j][i];
			}

		}
	}
	for (j =0; j < D; j++){
		if(numI > 0){
			retVals[j] =  retVals[j]/numI;
		}
	}
	return retVals;

}

//Runs until convergence.
MakeCodebook(){
	int nearest[N];
	int lastN[N];
	int numIts=0;
	while (1){
		int i;
		for (i = 0; i < N; i++){
			nearest[i] = computeNearestCentroid(i);
			//printf("nearest is %d \n", nearest[i]);
		}

		for(i =0; i < K; i++){
			entries[i] = meanOfPoints(defaults, nearest, i);
		}
		int countSame=0;
		int j;
		for(j = 0; j < N ; j++){
			if(nearest[j] == lastN[j]){
				countSame++;
			}
		}
		if(countSame == N){
			break;
		}
		for (j = 0; j < N; j++){
			lastN[j] = nearest[j];
		}
		numIts++;
	}
	int i;
	for( i = 0; i < K; i++){
		int j;
		for(j = 0; j< D-1; j++){
			printf("%f ",entries[i][j]);
		}
		printf("%f\n", entries[i][D-1]);
	}
	//printf("num its is %d\n", numIts);
}

//Populates needed data structures and reads in information.
int main(int argc, char* argv[]){
	if(argc < 4){
		printf("wrong number of arguments, desired usage is ./vq_train MFCCfile numberK dimVector\n");
		exit(1);
	}
	K = atoi( argv[2]);
	D = atoi(argv[3]);
	//Deal with files and init WILL END UP USING FGETS
	char *fname = argv[1];
	FILE *inputF ;
	if ((inputF = fopen(fname, "r")) == NULL){
		printf("Bad file name aborting file name provided is %s\n", fname);
		exit(1);
	}
	char buf[1024];
	entries = malloc( sizeof(entries) * K);
	defaults = malloc( sizeof(defaults));
	N = 0;
	//fills out the N sized array of data.
	while(fgets(buf, 1024, inputF) !=NULL){
		defaults[N] = malloc( sizeof(double) *D);
		char *token = strtok(buf, " \t");
		int k=0;
		while (token != NULL){
			defaults[N][k] = atof(token);
			token = strtok(NULL, " \t");
			k++;
		}
		N++;
		defaults = realloc(defaults, sizeof(defaults)* (N+1));
	}

	//Sets default values
	int i;
	for (i =0; i < K; i++){
		entries[i] = malloc( sizeof(double) *D);
		int m;
		for(m = 0; m < D; m++){
			entries[i][m] = defaults[i][m];
		}
	}
	MakeCodebook();
}