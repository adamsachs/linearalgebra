Adam Sachs
ajs2255
HW 4

1) My program takes the user input and samples the basis parabola described by the input at m equally spaced points [t(1), (t2), … , t(m)]. Because the assignment doesn't specify by how much we should space the points we are sampling, I decided to set the space between each point as 2. It then perturbs each data point by going through m times and finding a random number between -1 and 1 and adding it to a corresponding y value that is on the basis parabola. With this perturbed data, I fill in a vector b. I also fill in a matrix A, which is based solely on the t values. With this matrix and vector, I create (by matrix multiplication) an augmented matrix ('aug') that holds the square+invertible 3x3 matrix (A^T )A and then in its 4th column has the vector (A^T)b. I transform this matrix to rref through forward and backward elimination in order to solve for (x-hat) in the equation (A^T)A*(x-hat) = (A^T)b. In rref(aug), the 4th column (i.e. the transformed b-column) holds the (C, D, E) calculated coefficient values for the parabola of best fit. My program prints to an output file first the (A^T)A matrix, then the (A^T)b vector, and then the (C, D, E) calculated coefficients, and the original (C, D, E) coefficient values. 


3) My program is written in C and is titled parabolafitter.c. To compile it, type the command 'gcc parabolafitter.c.' An executable called 'a.out' will be created. In order to run the program, you need to type './a.out' with two command-line arguments, which define the names of they input and output files respectively. An example: './a.out input.txt output.txt'. In this case the program would take input from the file 'input.txt' and print output to the file 'output.txt'. The input file should be formatted as described in the assignment: the first line contains 'm', the second line contains 'C', the third line contains 'D', and the fourth 'E', where C+Dx+E*x*x is the formula of the parabola we wish to perturb. I've provided a sample input file titled 'input.txt' which creates a basis parabola with formula '2+3x+5*x*x' (i.e. C=2, D=3, E=5), with m=20.

Here is a sample output of a run with the sample input file:
(A^T)A:
20.000000 380.000000 9880.000000
380.000000 9880.000000 288800.000000
9880.000000 288800.000000 9002656.000000

(A^T)B:
50576.264000
1474345.320000
45898380.784000

C(new): 1.832145  C(original): 2
D(new): 2.983562  D(Original): 3
E(new): 5.000594  E(Original): 5

