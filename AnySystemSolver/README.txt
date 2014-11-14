Adam Sachs
ajs2255

This program solves a system of equations Ax = b, where A is a mxn matrix and b is an m dimensional vector. The format of the input file is described below.


My program is written in C and is titled 'axbsol.c'. To compile it, type the command 'gcc axbsol.c'. An executable called 'a.out' will be created. In order to run the program, you need to type './a.out' with two command-line arguments, which define the names of the input and output files. An example: './a.out input.txt output.txt'.  I'm providing a sample input file with a 4x7 rank 4 matrix, and a solution vector b. But in general, the input file format should be as follows: first line should be the m dimension, second line should be the n dimension, then after a line break put the coefficient matrix (rows separated by line breaks, and each element in a row separated by a single space). After a line break put the b vector matrix, with each value separated by a line break. If any of this is confusing, please refer to the sample input file. 