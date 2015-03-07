#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int main (int argc, char *argv[]){

	double **arrAInv;
	double *xvect;
	double *yvect;

	double vectsum, eigval; 
	int i, j, k;


	time_t t;


	//allocate memory for A^(-1) matrix
	arrAInv = malloc (10 * sizeof(double *));
	if (arrAInv==NULL){
		fprintf(stderr, "out of memory\n");
		return 0;
	}

	for (i=0; i<10; i++){
		arrAInv[i] = malloc ((10) * sizeof(double));
		if(arrAInv[i] == NULL){
			fprintf(stderr, "out of memory\n");
			return 0;
		}
	}

	//allocate memory for xvector
	xvect = malloc (10 * sizeof(double));
	if (arrAInv==NULL){
		fprintf(stderr, "out of memory\n");
		return 0;
	}

	//allocate memory for yvector
	yvect = malloc (10 * sizeof(double));
	if (arrAInv==NULL){
		fprintf(stderr, "out of memory\n");
		return 0;
	}

	//hard codes A^(-1) based on patterns in the matrix
	for (i =0; i<10; i++){
		for (j=0; j<=i; j++){
			arrAInv[i][j] = (10-i)*(j+1);
		}
		for (j=i+1; j<10; j++){
			arrAInv[i][j] = arrAInv[i][j-1] - (i+1);
		}
		for (j=0; j<10; j++){
			arrAInv[i][j] = arrAInv[i][j]/11;
		}
	}



	//initializes random number generator
	srand((unsigned) time(&t));
	vectsum = 0;

	//initalizes x as a random normalized vector
	for (i=0; i<10; i++){
		double r = (rand()%10) - 5;
		xvect[i] = r;
		vectsum += (xvect[i]*xvect[i]); 

	}

	for (i=0; i<10; i++){
		xvect[i] = xvect[i]/(sqrt(vectsum));

	}

	//15 iterations of power method
	for (i=0; i<15; i++){

		vectsum = 0;


		//each value of new yvector is dot product of row j of A w/ xvector
		for (j=0; j<10; j++){
			yvect[j] = 0;

			//dot product of row j of A w/ xvector
			for (k=0; k<10; k++){
				yvect[j] += arrAInv[j][k]* xvect[k];
			}

			vectsum += (yvect[j]*yvect[j]);
		}

		for (j=0; j<10; j++){
			xvect[j] = yvect[j]/(sqrt(vectsum));
		}

	}

	//initalizes eigenvalue at 0
	eigval = 0;

	//finds eigenvalue by computing dot product of xvector and A*xvector
	for (i=0; i<10; i++){

		//now we have yvect hold A*xvector
		//each value of new yvector is dot product of row j of A w/ x vector
		for (j=0; j<10; j++){
			yvect[j] = 0;

			//dot product of row j of A w/ xvector
			for (k=0; k<10; k++){
				yvect[j] += arrAInv[j][k]* xvect[k];
			}
		}

		eigval += (xvect[i] * yvect[i]);
	}

	//we take inverse of the largest eigenvalue of A(^-1) to get smallest eigenvalue of A
	eigval = 1/eigval;

	fprintf(stderr, "eigenvalue: %lf\n", eigval);


	free (xvect);
	free (yvect);

	for (i=0; i<10; i++){
		free(arrAInv[i]);
	}
	free (arrAInv);


}
