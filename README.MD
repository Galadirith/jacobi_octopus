Instructions

a simple “Hello World” type example for an Octopus workflow.
 
Instructions :
1.       cd Jacobi
2.       make
3.       run Jacobi_wf.sh

update the Makefile for compiler and mpi command in the Jacobi_wf.sh script.

Manager reguirement : 
S - jacobi
A - jacobi_analyse
dependency - x.mat file

CDO requirement : 
S - xlocal[12][12] partitioned across 4 ranks as xlocal[1:3][0:11] (includes halos, xlocal[0][0:11] & xlocal[4][0:11])
    xlocal decomposed in x direction only.
A - xlocal[12][12] partioned across 4 ranks as xlocal[0:2][0:11]
CDO^S == CDO^A 

Transport requirement:
S and A to run a separate mpi process (or a single mpi process). A to start after a delay. 
