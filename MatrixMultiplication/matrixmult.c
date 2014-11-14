#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	FILE *inputfile;
	FILE *outputfile;
	int n, k, i, j, z, y;
	double holder;

	double **arrayA;
	double **holderarray;

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

			//sets second character from input file to "k"
			fscanf(inputfile, "%d ", &k);

			//checks if given k value is >= 0
			if (k<0){
				fprintf(stderr, "k value must be >=0\n");
				return 0;
			}


			//allocates memory for 2d array (matrix) to hold A,
			//the input matrix
			arrayA = malloc (n * sizeof(double *));
			if (arrayA==NULL){
				fprintf(stderr, "out of memory\n");
				return 0;
			}
			for (i=0; i<n; i++){
				arrayA[i] = malloc(n * sizeof(double));
				if(arrayA[i] == NULL){
					fprintf(stderr, "out of memory\n");
					return 0;
				}
			}


			//fills in array with values given by input file
			for (i=0; i<n; i++){
				for (j=0; j<n; j++){
					fscanf(inputfile, "%lf ", &(arrayA[i][j]));
				}

			}

			//if k=0, skip all matrix multiplications
			//and just return A as it was given to us
			if (k==0){
				;
			}
			else{

				//if k>0, allocate another 2d array (matrix)
				//that will hold the product array
				holderarray = malloc (n * sizeof(double *));
				if (holderarray==NULL){
					fprintf(stderr, "out of memory\n");
					return 0;
				}
				for (i=0; i<n; i++){
					holderarray[i] = malloc(n * sizeof(double));
					if(holderarray[i] == NULL){
						fprintf(stderr, "out of memory\n");
						return 0;
					}
				}


				//this multiplication loop runs 'k' times,
				//i.e. we only perform k matrix multiplications
				for(y=0; y<k; y++){

					//this loop cycles thru all components of the product matrix,
					//computing the necessary dot products for each component
					for (i=0; i<n; i++){
						for (j=0; j<n; j++){

							//make sure holder resets to 0
							//before computing dot product
							holder = 0;

							//cycles thru corresponding row/column
							//to get different dot product terms
							for (z=0; z<n; z++){

								//add each individual product to the holder,
								//which will represent dot product
								holder += (arrayA[i][z]*arrayA[z][j]);
							}


							//dot product is then plugged in 
							//as the (i,j) component of resulting matrix
							holderarray[i][j] = holder;
						}
					}

					//we can now consider the product matrix, i.e. A^2
					//to be matrix A--so we plug in all values of A^2
					//into A, and clear holderArray for next multiplication
					for (i=0; i<n; i++){
						for (j=0; j<n; j++){
							arrayA[i][j] = holderarray[i][j];
							holderarray[i][j] = 0;
						}

					}
				}

				//frees memory allocated for the holder array
				for (i=0; i<n; i++){
					free(holderarray[i]);
				}
				free(holderarray);
			}


			//prints matrix A, which is now the solution matrix
			//to the given output file, in correcct format
			for (i=0; i<n; i++){
				for (j=0; j<n; j++){
					fprintf(outputfile, "%lf ", arrayA[i][j]);
				}
				fprintf (outputfile, "\n");

			}


			//frees memory allocated for arrayA
			for (i=0; i<n; i++){
				free(arrayA[i]);
			}
			free(arrayA);

			
		}
	}

	return 0;
}
