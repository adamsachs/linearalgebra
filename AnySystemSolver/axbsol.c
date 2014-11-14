#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	FILE *inputfile;
	FILE *outputfile;
	int n, m, i, j, k, rank, currow, currcolumn, nosol;
	int zerocounter, onecounter, freecolcounter, pivcolcounter, bcolcounter;

	double holder, l, pivotvalue;

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

			//sets first character from input file to "m"
			fscanf(inputfile, "%d ", &m);

			//sets second character from input file to "n"
			fscanf(inputfile, "%d ", &n);

			//allocates memory for 2d array (matrix, size m x n+1) to hold A,
			//the augmented matrix
			arrayA = malloc (m * sizeof(double *));


			if (arrayA==NULL){
				fprintf(stderr, "out of memory\n");
				return 0;
			}
			for (i=0; i<m; i++){
				arrayA[i] = malloc((n+1) * sizeof(double));
				if(arrayA[i] == NULL){
					fprintf(stderr, "out of memory\n");
					return 0;
				}
			}

			//fills in first n columns of the array with coefficient matrix
			for (i=0; i<m; i++){
				for (j=0; j<n; j++){
					fscanf(inputfile, "%lf ", &(arrayA[i][j]));
				}
			}


			//fills in nth column of array with rhs vector b
			for (i=0; i<m; i++){
				fscanf(inputfile, "%lf ", &(arrayA[i][n]));
			}
		}


/***************FORWARD ELIMINATION**********************/

		//initialize the rank and current column to 0
		rank = 0;
		currcolumn = 0;

		for (currow=0; currow<m; currow++){
			
	

			//checks if current pivot is 0; if it is, looks for a row swap
			while (arrayA[currow][currcolumn] == 0 && currcolumn != n){

				//goes below pivot, looking for a non-zero value
				for (j = currow+1; j<m; j++){
					holder = arrayA[j][currcolumn];
					if (holder !=0){

						//when a non-zero value is found, swap rows
						for (k=currow; k<=n; k++){
							holder = arrayA[currow][k];
							arrayA[currow][k] = arrayA[j][k];
							arrayA[j][k] = holder;
						}
					}
				}

				//if pivot value is still 0, i.e. no row swaps possible
				//then we are dealing with a free column, and skip to next
				if (arrayA[currow][currcolumn] == 0){
					currcolumn++;
				}

			}

			//if we have gone into the b column of the augmented matrix
			//this means we are done with forward elimination
			if (currcolumn == n){
				break;
			}			



			//now going to fill in 0's below pivots by row manipulation

			for (j = currow + 1; j<m; j++){
				l = arrayA[j][currcolumn]/arrayA[currow][currcolumn];
				for (k=0; k<=n; k++){
					arrayA[j][k] = arrayA[j][k] - l*(arrayA[currow][k]);
				}
			}

			//every time we reach the end of the loop,
			//it means we've dealt with a non-zero pivot,
			//so we increase the rank
			rank++;

		}





/***************BACK ELIMINATION**********************/




		for (currow = m-1; currow>=0; currow--){


			currcolumn = 0; 

			//if pivot value is 0, we have a free column, so skip to next
			while (arrayA[currow][currcolumn] == 0 && currcolumn != n){
				currcolumn ++;
			}

			//if we have gotten to a 0 in the last column,
			//we have a free row, so we go to next row
				if (currcolumn==n){
					continue;
				}


			//fill in 0's above pivots by row manipulation
			for (j = currow - 1; j>=0; j--){
				l = arrayA[j][currcolumn]/arrayA[currow][currcolumn];
				for (k=currcolumn; k<=n; k++){
					arrayA[j][k] = arrayA [j][k] - l*(arrayA[currow][k]);
				}
			}

		}




/***************DIVIDE BY PIVOT VALUES TO GET RREF**********************/


		//goes row by row to find each row's pivot (if it exists)

		for (currow = 0; currow < m; currow++){

			currcolumn = 0;

			while (arrayA[currow][currcolumn]==0){
				currcolumn++;
			}

			if (currcolumn==n){
				continue;
			}
			else{
				pivotvalue = arrayA[currow][currcolumn];
				for (i=0; i<=n; i++){
					arrayA[currow][i] = arrayA[currow][i]/pivotvalue;				}
			}
		}
	}









/***************PRINTING SOLUTIONS**********************/






	//this deals with an invertible matrix:
	//one solution, it's just the last column of the augmented matrix
	//i.e. the transformed vector b
	if (rank == n && rank == m){
		fprintf(outputfile, "there's a unique solution to the system. ");
		fprintf(outputfile, "it's the following vector: \n[\n");
		for (i=0; i<m; i++){
			fprintf(outputfile, "x%d: %lf\n", i+1, arrayA[i][n]);
		}
		fprintf(outputfile, "]\n");
	}





	//this deals with a full column rank matrix
	//checks to see if we have 0=0 at bottom of matrix
	//if not, then there is no solution to system
	//if yes, then gives the unique solution
	nosol = 0;
	if (rank == n && rank != m){

		//goes thru free rows, checking if b also =0
		//if b!=0, then we set nosol to true
		for (i=rank; i<m; i++){
			if (arrayA[i][n] != 0){
				nosol = 1;
			}
		}

		if (nosol){
			fprintf(outputfile, "there is no solution to the system.");
		}

		else{
			fprintf(outputfile, "there is a unique solution to the system. ");
			fprintf(outputfile, "it's the following vector:\n[\n");
			for (i=0; i<rank; i++){
				fprintf(outputfile, "x%d: %lf\n", i+1, arrayA[i][n]);
			}
			fprintf(outputfile, "]\n");
		}
	}






	//deals with matrices with rank less than n,
	//i.e. those without full column rank
	if (rank!=n){


		//if rank is less than m also, then we don't have full row rank,
		//so we need to check that b=0 in free rows
		if (rank !=m){

			//goes thru free rows, checking if b also =0
			//if b!=0, then we set nosol to true
			for (i=rank; i<m; i++){
				if (arrayA[i][n] != 0){
					nosol = 1;
				}
			}

			if (nosol){
				fprintf(outputfile, "there is no solution to the system.");
				return 0;
			}
		}


		//there are infinite solutions.
		fprintf(outputfile, "there are infinitely many solutions to the system.\n");
		fprintf(outputfile, "a particular solution, xp, is the following vector:\n[\n");

		//array to hold free column identifiers
		int freecols[n-rank];
		//array to hold pivot column identifies
		int pivcols[rank];

		//keeps track of how many free and pivot columns we've encountered
		freecolcounter = 0;
		pivcolcounter = 0;

		//keeps track of our place in the b column;
		//we go down one spot every time we encounter a non-free column
		bcolcounter = 0;



		//cycles through array to find free columns
		for (i=0; i<n; i++){
			zerocounter = 0;
			onecounter = 0;
			for (j=0; j<m; j++){

				if (arrayA[j][i] == 0){
					zerocounter++;
				}
				if (arrayA[j][i] == 1){
					onecounter++;
				}
			}

			//any column that doesn't have m-1 zeros and 1 one is a free column
			//so we add it to the array that identifies freecolumns
			if (zerocounter != m-1 || onecounter != 1){
				fprintf(outputfile, "x%d: 0\n", i+1);
				freecols[freecolcounter] = i;
				freecolcounter++;
			}

			//if it's a non-free column, then it's variable isn't free
			//and its value in the solution will be the next value
			//in the b column. this means we go down 1 spot in the b column
			//and we add this column to our array of pivot columns
			else{
				fprintf(outputfile, "x%d: %lf\n", i+1, arrayA[bcolcounter][n]);
				pivcols[pivcolcounter] = i;
				pivcolcounter++;
				bcolcounter++;
			}
		}

		fprintf(outputfile, "]\n");
		fprintf(outputfile, "the %d special solution(s) (i.e. the vector(s) ", n-rank);
		fprintf(outputfile, "that span N(A)) are: \n");

		//goes thru the free columns of arrayA to get our special solutions
		for (i=0; i<freecolcounter; i++){
			currcolumn = freecols[i];

			//this array holds the current special solution we're working on
			double nullvector[n];

			//set all free variables in nullvector to 0
			for (j=0; j<freecolcounter; j++){
				nullvector[freecols[j]] = 0;
			}

			//set component corresponding to current special solution = 1
			nullvector[currcolumn] = 1;

			//puts the values of free column we're working on into
			//the corresponding spots for pivot variables in special solution
			//this works b/c arrayA is in rref
			for (j=0; j<rank; j++){
				nullvector[pivcols[j]] = -1*arrayA[j][currcolumn];
			}

			fprintf(outputfile, "[\n");
			for (j=0; j<n; j++){
				fprintf(outputfile, "x%d: %lf\n", j+1, nullvector[j]);
			}
			fprintf(outputfile, "]\n");
		}
	}



/*********************frees memory allocated for arrayA******************/
		for (i=0; i<m; i++){
			free(arrayA[i]);
		}
		free(arrayA);


	return 0;
}




