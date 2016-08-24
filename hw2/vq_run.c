//David Shagam
//vq_run
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
double** codebook;
double** defaults;

//Computers the centroid given the index that is being used.
int computeNearestCentroid(int ind){
	int nearestInd= 0;
	double lastcalc = DBL_MAX;
	double calc=0;
	int i;
	for(i = 0; i < K; i++){
		int j;
		for(j = 0; j < D; j++){
			double temp = (defaults[ind][j] - codebook[i][j] );
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

//Loads files into memory. populating information.
int main(int argc, char *argv[]){
	if(argc < 5){
		printf("wrong number of arguments, desired usage is ./vq_run MFCCfile codebookfile numberK dimVector\n");
		exit(1);
	}
	K = atoi( argv[3]);
	D = atoi(argv[4]);
	//Deal with files and init WILL END UP USING FGETS
	char *fname = argv[1];
	FILE *inputF ;
	if ((inputF = fopen(fname, "r")) == NULL){
		printf("Bad file name aborting file name provided is %s\n", fname);
		exit(1);
	}
	char *cname = argv[2];
	FILE *inputC;
	if((inputC = fopen(cname, "r"))==NULL){
		printf("Bad file name aborting file name provided is %s\n", cname);
		exit(1);
	}
	char buf[1024];
	codebook = malloc( sizeof(codebook) * K);
	int j;
	for (j = 0; j < K; j++){
		codebook[j] = malloc ( sizeof(double) * D);
	}
	defaults = malloc( sizeof(defaults));
	N = 0;
	//Fills the entire N sized values to map to codebook
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
	int ind = 0;
	//Filles the codebook.
	while(fgets(buf, 1024, inputC) != NULL){
		char *token = strtok(buf, " \t");
		int iter =0;
		while(token != NULL){
			codebook[ind][iter] = atof(token);
			iter++;
			token = strtok(NULL, " \t");
		}
		ind++;

	}

	int nearest[N];
	int i;
	//Computers the nearest and prints the thing it's supposed to match to.
	for(i = 0; i < N; i++){
		nearest[i] = computeNearestCentroid(i);
		printf("%d\n", nearest[i]);
	}

}