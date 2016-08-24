//David Shagam
//vq_train
//assignment 2
//csci 479

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>

//Removes any line containing a - and removes the leading line numbers.
//Reformats so it's only using spaces between floats.
int main(int argc, char *argv[]){
	char buff[1024];
	while(fgets(buff,1024, stdin)!= NULL){
		if(buff[0] != '-'){
			char* token;
			token = strtok(buff, " \t");
			while(token != NULL){
				int len = strlen(token);
				if(token[len-1] != ':'){
					printf("%s ", token);
				}
				token = strtok(NULL, " \t");
			}
			//printf("\n");
		}
	}
}