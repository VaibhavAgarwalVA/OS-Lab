# Operating-Systems-Lab
Codes written for the Operating Systems Lab course offered at IIT Kharagpur during Spring 2017

## Assignment 1
See the complete problem statement [here](http://cse.iitkgp.ac.in/~agupta/OSLab/Assgn1.pdf).

Brief descriptions are as follows:

**search.c**: Linear Search over an array using parallelization

**sort1.c**: Sort on integers in file specified by command line args

**xsort.c**: Sort on integers in file specified by command line args in xterm environment.

## Assignment 2 Part 1
See the complete problem statement [here](http://cse.iitkgp.ac.in/~agupta/OSLab/Assgn2.pdf).

Brief description is as follows:

**fcopy.c**: Aim is to understand interprocess communication (IPC) through copying the content of one file to other using 2 system pipes.

## Assignment 2 Part 2
See the complete problem statement [here](http://cse.iitkgp.ac.in/~agupta/OSLab/Assgn2.pdf).

Brief descriptions are as follows:

**shell.c**: A minishell with command set described in the assignment problem statement.

**run.c**:  Runs the shell as a child process.

## Assignment 3 Part 1
See the complete problem statement [here](http://cse.iitkgp.ac.in/~agupta/OSLab/Assgn3.pdf)

Brief description is as follows:

**prodcons.c**: Solution for the m-producer n-producer problem.

## Assignment 3 Part 2
See the complete problem statement [here](http://cse.iitkgp.ac.in/~agupta/OSLab/Assgn3.pdf)

Brief description is as follows:

**X.c**: A process X, when started, first loads all the records in shared memory from a file whose name is passed to it as a command line argument. It then goes to sleep, waking up periodically (say every 5 seconds) to check if any of the data have been modified. If yes, it writes the entire content of the shared memory back to the file. The process X runs forever. 

**Y.c**: Any number of other processes can query and update the data by reading/writing the shared memory.

## Assignment 4 Part 1
See the complete problem statement [here](http://cse.iitkgp.ac.in/~agupta/OSLab/Assgn4.pdf)

Brief description is as follows:

**shuffle.c**: Read a file name as command line argument which stores a nxn matrix, and use threads for computing k-circular horizontal and vertical shifts of the matrix.


## Assignment 4 Part 2
See the complete problem statement [here](http://cse.iitkgp.ac.in/~agupta/OSLab/Assgn4.pdf)

Brief descriptions are as follows:

**A.c**: The client process that sends periodic requests to book tickets.

**B.c**: The server process that uses threads to cater to these requests, while preventing a high number of active threads.

Process B must be ran first. Process A is killed whenever process B is killed. 
