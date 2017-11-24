#include "api.hpp"
#include <stdio.h>
#include <math.h>
#include "mpi.h"
#include <cstdlib>
#include <string>

/* This example handles a 12 x 12 mesh, on 4 processors only. */

int main(int argc,char *argv[])
{
    int        rank, value, size, errcnt, toterr, i, j, itcnt;
    MPI_Status status;
    MPI_File fh;
    MPI_Offset my_offset, my_current_offset, total_number_of_bytes;
    char filename[6] = "x.mat";
    double     norm=0.0, totalnorm=0.0;

    // Prepare transport
    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    if (size != 4) MPI_Abort( MPI_COMM_WORLD, 1 );

    double *xlocal;
    Octopus *octopus = new Octopus();
    while (!octopus->getArray(&xlocal)) {
        for(j=0;j<36;j++) {
            norm+= xlocal[j]*xlocal[j];
        }
    }
    
    // Print array head for debug
    std::string row;
    for (int i=0; i<3; i++)
    {
        row = "";
        for (int j=0; j<12; j++)
          row += std::to_string(xlocal[i*12 + j]) + ", ";
        fprintf(stderr, "rank: %s\n", row.c_str());
    }
    delete octopus;

    MPI_Allreduce( &norm, &totalnorm, 1, MPI_DOUBLE, MPI_SUM,MPI_COMM_WORLD );

    printf("rank %d : %f\n",rank,totalnorm);

    MPI_Finalize();

}
