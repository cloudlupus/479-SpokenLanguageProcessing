/*
David Shagam
Homework 3
Spoken Language Processing CSCI 479
Program to compute P(Y_1 ^T), optimal path, and generate a sequence 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define DEBUG 0
#define DEBUG2 0

void forward();
void printForward();
void backward();
void populateG(double **gamma);
void populateE(double ***epsilon);
void train();
void sample();
void printVit();
void viturbi();

double *Pi;
int numN;//Num N;
double **A; //NxN
double **B;
int numB;//Used for BxN
double **Alpha;
double **Beta;
int numSeq; // used for T
int **backtrack;
int *Sequence;

//return matrixes
double *newPi;
double **newA;
double **newB;
//nums and denoms
double *ANum;
double *ADenom;
double *BNum;
double *BDenom;

void backward(){
	Beta = malloc( sizeof(double)* numN);
	int i;
	for(i = 0; i< numN; i++){
		Beta[i] = malloc( sizeof(double) * numSeq);
		Beta[i][numSeq-1] = 1;
	}
	//For every column.
	for(i = numSeq-2; i>=0; i--){
		//For each state
		int j;
		for(j = 0; j< numN; j++){
			//Sum of previous column
			double sum = 0;
			int y;
			//calc sum from alpha down the side
			for(y = 0; y < numN; y++){ //Running sum of previous values
				sum += Beta[y][i+1]*A[y][j]*B[j][(Sequence[i+1]-1)];
			}
			Beta[j][i] = sum; //set into beta
		}


	}

}

void populateG(double **gamma){
	int i;
	for(i = 0; i < numN; i++){
		gamma[i] = malloc( sizeof(double) * numSeq);
	}
	double constant = 0;
	for(i = 0; i < numN; i++){
		//for every state.
		constant += Alpha[i][numSeq-1];
	}
	//for every T
	for(i = 0; i < numSeq; i++){
		//for every n
		int j;
		for(j = 0; j < numN; j++){
			gamma[j][i] = Alpha[j][i] * Beta[j][i] / constant;
		}

	}

}

void populateE(double ***epsilon){
	double constant = 0;
	int i;
	for(i = 0; i < numN; i++){
		//for every state.
		constant += Alpha[i][numSeq-1];
	}
	//set up the epsilon 3d matrix.
	for(i = 0; i< numSeq; i++){
		epsilon[i] = malloc( sizeof(double)* numN);
		int j;
		for(j = 0; j < numN; j++){
			epsilon[i][j] = malloc( sizeof(double) * numN);
		}
	}
	//for all Seq
	int t;
	for(t = 0; t < numSeq-1; t++){
		int i;
		//for all states
		for(i =0; i < numN; i++){
			int j;
			//for all states again.
			for(j = 0; j< numN; j++){
				epsilon[t][i][j] = Alpha[i][t] *A[i][j] * B[j][Sequence[t+1]-1] * Beta[j][t+1] /constant;
			}
		}
	}


}

void train(char* outPi, char* outA, char* outB){
	forward();
	backward();
	double **gamma;
	gamma = malloc( sizeof(double) * numN);
	double ***epsilon;
	epsilon = malloc( sizeof(double) * numSeq);

	populateE(epsilon);
	populateG(gamma);

	//Build new estimates.
	
	int i;
	for(i =0; i < numN; i++){
		newPi[i] += gamma[i][1];		
		newA[i]= malloc( sizeof(double) * numN);
		newB[i] = malloc( sizeof(double) *numB);
	}

	for(i = 0; i< numN; i++){
		int j;
		for(j = 0; j<numN; j++){
			int t;
			double gammaSum = 0;
			double epsSum = 0;
			for(t=0; t< numSeq-1; t++){
				epsSum += epsilon[t][i][j];
				gammaSum += gamma[i][t];
			}
				newA[i][j] = epsSum/gammaSum;
		}
	}


	//Print to files.
}

//Generates a hidden and observed sequence of length len.
void sample(int len){
	//seeds the random so it's not the same every time
	srandom(clock());
	//Makes the value between 0 and 1
	double val = ((double)random())/RAND_MAX;
	//iterator
	int i;
	//last value for traversals
	int lastX;

	//Gen first X
	double minBound = 0;
	double maxBound = 0;
	for(i =0; i< numN; i++){
		maxBound += Pi[i]; //increment bound
		if(minBound != 0){//edge cases
			if( minBound < val && val <=  maxBound){
				lastX = i;
				fprintf(stderr,"%d\n",i+1);
				break;
			}
		} else{
			if( val <= maxBound){
				lastX = i;
				fprintf(stderr, "%d\n", i+1);
				break;
			}
		}
		minBound = maxBound;
	}

	//Gen first Y
	maxBound = 0;
	minBound = 0;
	val = ((double)random())/RAND_MAX;
	for (i = 0; i < numN; i++){
		maxBound += B[lastX][i];
		if(minBound !=0){
			if(minBound < val && val <= maxBound){
				printf("%d\n",i+1);
				break;
			}
		} else {
			if(val <= maxBound){
				printf("%d\n",i+1);
				break;
			}
		}
		minBound = maxBound;
	}

	//Gen rest of sequence
	int j;
	for(j = 0; j < len; j++){
		//reset book keeping
		maxBound = 0;
		minBound = 0;
		val = ((double)random())/RAND_MAX;

		//Gen X
		for(i = 0; i< numN; i++){
			maxBound += A[lastX][i];
			if(minBound !=0){
				if(minBound < val && val <= maxBound){
					lastX = i;
					fprintf(stderr, "%d\n",i+1);
					break;
				}
			} else{
				if(val <= maxBound){
					lastX = i;
					fprintf(stderr, "%d\n", i+1);
					break;
				}
			}
			minBound = maxBound;
		}


		//Gen Y
		maxBound = 0;
		minBound = 0;
		val = ((double)random())/RAND_MAX;
		for(i = 0; i < numN; i++){
			maxBound += B[lastX][i];
			if( minBound !=0){
				if(minBound < val && val <= maxBound){
					printf("%d\n", i+1);
					break;
				}
			} else {
				if(val <= maxBound){
					printf("%d\n", i+1);
					break;
				}
			}
			minBound = maxBound;
		}

	}

}

//Recursive way to print the path
//used to print in the proper order.
void printVit(int i, int j){
	if(j-1 >= 0){
		printVit( backtrack[i][j],j-1);
		if(j == (numSeq-1)){
			printf("%d\n", i+1);
		} else {
			printf("%d ", i+1);
		}
	} else {
		printf("%d ", i+1);
	}

}

//Finds the optimal path of a set of observable states
//Prints out path.
void viturbi(){
	Alpha = malloc( sizeof(double)* numN);
	backtrack = malloc( sizeof(int) * numN);
	int i;
	for(i = 0; i< numN; i++){
		backtrack[i] = malloc( sizeof(int) * (numSeq-1));
		backtrack[i][0] = i;
		Alpha[i] = malloc( sizeof(double) * numSeq);
		Alpha[i][0] = Pi[i]*B[i][Sequence[0]-1];
	}
	//Calculate max path.
	//Across the top of Alpha
	for(i = 1; i<numSeq; i++){
		int j;
		//for each State
		for(j=0; j<numN; j++){
			double largest = 0;
			double cur = 0;
			int y;
			//calc sum from alpha down the side
			for(y = 0; y < numN; y++){
				cur = Alpha[y][i-1]*A[y][j];
				if(cur > largest){ //Find the max and only use that.
					largest = cur;
					backtrack[j][i] = y;
				}
			}
			Alpha[j][i] = largest * B[j][Sequence[i]-1];
		}
	}
	//Find max;
	int chain;
	double max = 0;
	for(i = 0; i < numN; i++){
		double cur = Alpha[i][numSeq-1];
		if(cur > max){
			chain = i;
			max = cur;
		}

	}
	//Print backtracking with max found
	printVit(chain,numSeq-1);

}

//Prints P(Y_1^T) for a set of observed states
void forward(){
	Alpha = malloc( sizeof(double)* numN);
	int i;
	for(i = 0; i< numN; i++){
		Alpha[i] = malloc( sizeof(double) * numSeq);
		Alpha[i][0] = Pi[i]*B[i][Sequence[0]-1];
	}
	//Across the top of Alpha
	for(i = 1; i<numSeq; i++){
		int j;
		//for each State
		for(j=0; j<numN; j++){
			double sum = 0;
			int y;
			//calc sum from alpha down the side
			for(y = 0; y < numN; y++){ //Running sum of previous values
				sum += Alpha[y][i-1]*A[y][j];
			}
			sum = sum * B[j][Sequence[i]-1]; // corrective factor.
			Alpha[j][i] = sum; //set into alpha
		}
	}
	
}

void printForward(){
	int i;
	double printSum =0;
	for(i = 0; i <numN; i++){
		printSum += Alpha[i][numSeq-1];
	}
	//Print with formatting
	printf("%.5f\n", log10f(printSum));

}


//Takes in arguments to either do the forward algorithm, veturbi,
//Or generate states based on HMM
int main(int argc, char *argv[]){
	if(argc < 6){
		printf("Insufficient arguments requires -flag, seqeuence file, pi file, a file, b file\n");
		exit(EXIT_FAILURE);
	}
	//Load all the tables
	Pi = malloc( sizeof(double));
	ANum = malloc( sizeof(double));
	ADenom = malloc( sizeof(double));
	BNum = malloc( sizeof(double));
	BDenom = malloc( sizeof(double));

	char *PiName = argv[3];
	char *AName = argv[4];
	char *BName = argv[5];
	FILE *inputFile;
	//Load Pi
	if((inputFile = fopen(PiName, "r"))==NULL){
		printf(" Invalid file name for Pi Table\n");
		exit(EXIT_FAILURE);
	}
	char Buff[1024];
	while( fgets(Buff, 1024, inputFile) !=NULL){
		Pi = realloc(Pi, sizeof(double) * (numN+1));
		Pi[numN] = powf(10, atof(Buff));
		if(atof(Buff) == -99){
			Pi[numN] = 0;
		}
		numN++;
	}
	//Normalize Pi
	double sum =0;
	int norm;
	for(norm = 0; norm < numN; norm++){
		sum += Pi[norm];
	}
	for(norm = 0; norm < numN; norm++){
		Pi[norm] = Pi[norm]/sum;
	}

	//Loads A matrix
	if((inputFile = fopen(AName, "r"))==NULL){
		printf(" Invalid file name for A Matrix\n");
		exit(EXIT_FAILURE);
	}
	A = malloc( sizeof(double)* numN);
	int i;
	for(i =0 ; i< numN; i++){
		A[i] = malloc( sizeof(double) * numN);
	}
	for(i = 0; i < numN; i++){
		fgets(Buff, 1024, inputFile);
		int j;
		char* token = strtok(Buff, " \t");
		A[i][0] = powf(10,  atof(token));
		if(atof(token) == -99) {
			A[i][0] = 0;
		}
		for(j = 1; j< numN; j++){
			token = strtok(NULL, " \t");
			if(token != NULL){
				A[i][j] = powf(10, atof(token));
				if(atof(token) == -99){
					A[i][j] = 0;
				}
			}

		}
	}
	//Normalize A
	for(norm = 0; norm < numN; norm++){
		sum = 0;
		for(i = 0; i < numN; i++){
			sum += A[i][norm];
		}
		for(i = 0; i<numN; i++){
			A[i][norm] = A[i][norm]/sum;
		}
	}

	//Populate B matrix
	B= malloc( sizeof(double) * numN);
	if((inputFile = fopen(BName, "r"))==NULL){
		printf(" Invalid File name for B Matrix\n");
		exit(EXIT_FAILURE);
	}
	numB=0;
	fgets(Buff, 1024, inputFile);
	char* token2 = strtok(Buff, " \t");
	B[0]= malloc( sizeof(double));
	while( token2 != NULL){
		numB++;
		B[0] = realloc(B[0], sizeof(double) * numB);
		B[0][numB-1] = powf(10, atof(token2));
		if(atof(token2) == -99){
			B[0][numB-1] = 0;
		}
		token2 = strtok(NULL, " \t");
	}
	for (i = 1; i < numN; i++){
		fgets(Buff, 1024, inputFile);
		B[i]= malloc( sizeof(double) * numB);
		token2 = strtok(Buff, " \t");\
		int ind=0;
		while(token2 !=NULL){
			B[i][ind] = powf(10, atof(token2));
			if(atof(token2)== -99){
				B[i][ind] = 0;
			}
			ind++;
			token2 = strtok(NULL, " \t");
		}
		
	}
	//Normalize B
	for(norm = 0; norm < numB; norm++){
		sum = 0;
		for(i = 0; i < numN; i++){
			sum+= B[i][norm];
		}
		for(i = 0; i < numN; i++){
			B[i][norm] = B[i][norm]/sum;
		}
	}


	#if DEBUG
		printf("pi is\n");
		for(i =0; i < numN; i++){
			printf("%f\n",Pi[i]);
		}

		printf("\nA is\n");
		for(i =0; i < numN; i++){
			int j;
			for (j = 0; j < numN; j++){
				printf("%f ",A[i][j]);
			}
			printf("\n");
		}
		printf("\nB is\n");
		for(i = 0; i < numN; i++){
			int j;
			for(j = 0; j< numB; j++){
				printf("%f ",B[i][j]);
			}
			printf("\n");
		}

		printf("num N is %d and num B is %d\n", numN, numB);
	#endif


	//Setup new pi
	newPi = malloc( sizeof(double)* numN);
	for(i = 0; i< numN; i++){
		newPi[i] = 0;
	}


	//Check for sample
	if(strcmp(argv[1], "-sample")==0){
		sample(atoi(argv[2]));
	
	}else {
		//handle seq file
		if((inputFile = fopen(argv[2], "r")) == NULL){
			printf(" Invalid File name for Sequences\n");
			exit(EXIT_FAILURE);
		}

		Sequence = malloc( sizeof(int));
		int totalSequences = 0;
		while(fgets(Buff, 1024, inputFile)!= NULL){
			char *token = strtok(Buff, " \t");
			numSeq = 0;
			totalSequences++;
			while(token != NULL){
				Sequence[numSeq] = atoi(token);
				numSeq++;
				Sequence = realloc(Sequence, sizeof(int) * (numSeq+1));
				token = strtok(NULL, " \t");

			}
			if( strcmp(argv[1], "-eval")==0){
				forward();
				printForward();

			} else if(strcmp(argv[1], "-decode") ==0){
				viturbi();
			} else if( strcmp(argv[1], "-train") ==0){
				//Traninig algorithms.
				if(argc < 9){
					printf("Insufficient arguments\n");
					exit(EXIT_FAILURE);
				}
				train(argv[7], argv[8], argv[9]);

			}
		}
		//calculate and print out trained tables.
		if(strcmp(argv[1], "-train") == 0){

		}

	}
	return 0;
}