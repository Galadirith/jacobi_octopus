#include <stdio.h>
#include <math.h>
#include "mpi.h"

/* This example handles a 12 x 12 mesh, on 4 processors only. */

int main(int argc,char *argv[])
{
    int        rank, value, size, errcnt, toterr, i, j, itcnt;
    MPI_Status status;
    MPI_File fh;
    MPI_Offset my_offset, my_current_offset, total_number_of_bytes;
    char filename[6] = "x.mat";
    double     norm=0.0, totalnorm=0.0;
    double     xlocal[(12/4)][12];

    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    if (size != 4) MPI_Abort( MPI_COMM_WORLD, 1 );

    MPI_File_open(MPI_COMM_WORLD, filename,MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    MPI_File_get_size(fh, &total_number_of_bytes);
    my_offset = (MPI_Offset) rank * total_number_of_bytes/size;
    MPI_File_seek(fh, my_offset, MPI_SEEK_SET);
    MPI_File_read(fh,xlocal, 3*12, MPI_DOUBLE, &status);
    MPI_File_close(&fh);

    for(i=0;i<3;i++)
        for(j=0;j<12;j++)
            norm+= xlocal[i][j]*xlocal[i][j];

    MPI_Allreduce( &norm, &totalnorm, 1, MPI_DOUBLE, MPI_SUM,MPI_COMM_WORLD );

    printf("rank %d : %f\n",rank,totalnorm);
    MPI_Finalize();

}
