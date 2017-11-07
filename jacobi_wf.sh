rm x.mat
mpiexec -n 4 ./jacobi
mpiexec -n 4 ./jacobi_analyse

