#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	FILE *inputfile;
	FILE *outputfile;
	int n, m, i, j, k;
	double l;

	double **arrayA;

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

			//sets first character from input file to "n"
			fscanf(inputfile, "%d ", &n);

			//sets second character from input file to "m"
			fscanf(inputfile, "%d ", &m);



			//allocates memory for 2d array (matrix) to hold A,
			//the augmented matrix
			arrayA = malloc (n * sizeof(double *));
			if (arrayA==NULL){
				fprintf(stderr, "out of memory\n");
				return 0;
			}
			for (i=0; i<n; i++){
				arrayA[i] = malloc((n+1) * sizeof(double));
				if(arrayA[i] == NULL){
					fprintf(stderr, "out of memory\n");
					return 0;
				}
			}


			//fills in first n columns of the array with coefficient matrix
			for (i=0; i<n; i++){
				for (j=0; j<n; j++){
					fscanf(inputfile, "%lf ", &(arrayA[i][j]));
				}
			}

			//fills in nth column of array with rhs vector b
			for (i=0; i<n; i++){
				fscanf(inputfile, "%lf ", &(arrayA[i][n]));
			}


			//forward elimination
			for (k=0; k<n; k++){
				
				//goes thru all rows below pivot
				for (i=k+1; i<n; i++){

					l = arrayA[i][k]/arrayA[k][k];

					//skips to only the non-zero values in array
					for (j=i-(m/2); j<=i+(m/2); j++){

						//this is to avoid operating on non-existant elements
						if (j<0 || j>n-1){
							continue;
						}
						arrayA[i][j] = arrayA[i][j] - (l * arrayA[k][j]);
					}

					//adjusts b value
					arrayA[i][n] = arrayA[i][n] - (l * arrayA[k][n]);

					//fill in lower triangle with 0's
					arrayA[i][k] = 0;
				}
			} 

			//backward elimination
			for (k=n-1; k>0; k--){

				//goes thru all rows above pivot
				for (i=k-1; i>=0; i--){

				l = arrayA[i][k]/arrayA[k][k];

					//only goes thru the remaining non-zero elements
					//in each row
					for (j=i+(m/2); j>=i; j--){

						//to avoid operating on non-existent elements
						if (j>n-1){
							continue;
						}

						arrayA[i][j] = arrayA[i][j] - (l*arrayA[k][j]);
					}

					//adjusts b value
					arrayA[i][n] = arrayA[i][n] - (l*arrayA[k][n]);
				}
			}


			//we are left with diagonal matrix with pivot values
			//want to divide each row by the pivot value to get identity matrix
			for (k=0; k<n; k++){
				double pivot = arrayA[k][k];
				arrayA[k][n] = (arrayA[k][n]/pivot);
				arrayA[k][k] = (arrayA[k][k]/pivot); 
			}
			

			for (i=0; i<n; i++){
				for (j=0; j<(n+1); j++){
					fprintf(outputfile, "%lf ", arrayA[i][j]);
				}
				fprintf (outputfile, "\n");

			}



		}
	}



	return 0;
}
