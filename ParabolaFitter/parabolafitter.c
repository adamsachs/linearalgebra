#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]){

	FILE *inputfile;
	FILE *outputfile;
	int m, C, D, E, i, j, k;

	double *arrayt;
	double *arrayB;
	double **arrayA;
	double **aug;

	time_t t;



	if (argc !=3){
		printf("usage: %s input_file output_file\n", argv[0]);
	}

	//only run program if command line has 3 arguments
	else{

		//reads from file given by first argument
		inputfile = fopen(argv[1], "r");

		//writes to file given by second argument
		outputfile = fopen(argv[2], "w");

		//inputfile reads 0 with read failure
		if (inputfile == 0 || outputfile == 0){
			printf("couldn't open file \n");
		}
		else{

			//sets first character from input file to "m"
			fscanf(inputfile, "%d ", &m);

			//sets second character from input file to "C"
			fscanf(inputfile, "%d ", &C);

			//sets third character from input file to "D"
			fscanf(inputfile, "%d", &D);

			//sets fourth character from input file to "E"
			fscanf(inputfile, "%d", &E);


			//allocates memory for 1d array (matrix, size m x 1) to hold t,
			//the vector that has the t values t(1), t(2), ... , t(m)
			arrayt = malloc (m * sizeof(double));
			if (arrayt==NULL){
				fprintf(stderr, "out of memory\n");
				return 0;
			}



			//allocates memory for 2d array (matrix, size m x 3) to hold A
			arrayA = malloc (m * sizeof(double *));
			if (arrayA==NULL){
				fprintf(stderr, "out of memory\n");
				return 0;
			}

			for (i=0; i<m; i++){
				arrayA[i] = malloc ((3) * sizeof(double));
				if(arrayA[i] == NULL){
					fprintf(stderr, "out of memory\n");
					return 0;
				}
			}



			//allocates memory for 1d array (matrix, size m x 1) to hold vector b
			arrayB = malloc (m * sizeof(double));
			if (arrayB==NULL){
				fprintf(stderr, "out of memory\n");
				return 0;
			}



			//allocates memory for 2d array (matrix, size 3 x 4) to hold aug,
			//the augmented matrix that holds A^T*A (3x3) and A^T*B(3x1)
			aug = malloc(3* sizeof(double*));
			if (aug==NULL){
				fprintf(stderr, "out of memory\n");
				return 0;
			}

			for (i = 0; i<3; i++){
				aug[i] = malloc((4) * sizeof(double));
				if (aug[i] == NULL){
					fprintf(stderr, "out of memory\n");
					return 0;
				}
			}
		}



		/***************** CREATE DATA + FILL MATRICES ******************/

		//creates points t(1), t(2), ... , t(m),
		//such that they are each spaced equally by 2
		//we start with t(1) = 0 for all data sets and go forward from there
		for (i=0; i<m; i++){
			arrayt[i] = i*2;
		}


		//initializes random number generator
		srand((unsigned) time(&t));


		//for vector b
		for (i=0; i<m; i++){

			//data point on the parabola
			double b = C + (D * arrayt[i]) + (E * arrayt[i] * arrayt[i]);
			

			//generate random number to perturb data
			double r = (rand()%1000);
			r = r/500;
			r = r-1;

			//perturb data point
			b = b + r;

			arrayB[i] = b;
		} 


		//for matrix A
		for (i = 0; i<m; i++){


			//fill in first column with 1's
			arrayA[i][0] = 1;

			//fill in second column with t(i)
			arrayA[i][1] = arrayt[i];

			//fill in third column with t(i)^2
			arrayA[i][2] = arrayt[i]*arrayt[i];


		}



		/************ MULTIPLY THROUGH BY A^T, store in aug ******************/

		//first do the left side, i.e. multiply A^T by A
		fprintf(outputfile, "(A^T)A:\n");
		for (i = 0; i<3; i++){
			for (j = 0; j < 3; j++){
				aug[i][j] = 0;
				for (k = 0; k<m; k++){
					aug[i][j] += arrayA[k][i]*arrayA[k][j];
				}
				fprintf(outputfile, "%lf ", aug[i][j]);
			}
			fprintf(outputfile, "\n");
		}
		fprintf(outputfile, "\n");


		//now do the right side, i.e. multiply A^T by B
		fprintf(outputfile, "(A^T)B:\n");
		for (i = 0; i<3; i++){
			aug[i][3] = 0;
			for (j = 0; j<m; j++){
				aug[i][3] += arrayA[j][i] * arrayB[j];
			}
			fprintf(outputfile, "%lf\n", aug[i][3]);
		}
		fprintf(outputfile, "\n");





		/***************FORWARD ELIMINATION**********************/


		for (i =0; i<3; i++){
			
	

			//checks if current pivot is 0; if it is, looks for a row swap
			while (aug[i][i] == 0){

				//goes below pivot, looking for a non-zero value
				for (j = i+1; j<3; j++){
					double holder = aug[j][i];
					if (holder !=0){

						//when a non-zero value is found, swap rows
						for (k=i; k<4; k++){
							holder = aug[i][k];
							aug[i][k] = aug[j][k];
							aug[j][k] = holder;
						}
						break;
					}
				}

			}
		


			//now going to fill in 0's below pivots by row manipulation

			for (j = i + 1; j<3; j++){
				double l = aug[j][i]/aug[i][i];
				for (k=0; k<4; k++){
					aug[j][k] = aug[j][k] - l*(aug[i][k]);
				}
			}

		}





		/***************BACK ELIMINATION**********************/




		for (i = 2; i>=0; i--){

			//fill in 0's above pivots by row manipulation
			for (j = i - 1; j>=0; j--){
				double l = aug[j][i]/aug[i][i];
				for (k=i; k<4; k++){
					aug[j][k] = aug[j][k] - l*(aug[i][k]);
				}
			}

		}


		/***************DIVIDE BY PIVOT VALUES TO GET RREF**********************/



		for (i=0; i < 3; i++){
			
				aug[i][3] = aug[i][3]/aug[i][i];			
		}


		/************************ PRINT *********************************/

		fprintf(outputfile, "C(new): %lf  C(original): %d\n", aug[0][3], C);
		fprintf(outputfile, "D(new): %lf  D(Original): %d\n", aug[1][3], D);
		fprintf(outputfile, "E(new): %lf  E(Original): %d\n", aug[2][3], E);


		/********************** FREE MEMORY ****************************/

		free(arrayt);
		free(arrayB);

		for (i=0; i<m; i++){
			free(arrayA[i]);
		}
		free (arrayA);
		
		for (i=0; i<3; i++){
			free(aug[i]);
		}
		free(aug);

	}

	return 0;
}

