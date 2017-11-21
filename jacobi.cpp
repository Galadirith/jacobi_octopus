#include <decaf/decaf.hpp>
#include <bredala/data_model/pconstructtype.h>
#include <bredala/data_model/arrayconstructdata.hpp>
#include <cci.h>
#include <bredala/data_model/boost_macros.h>

#include <stdio.h>
#include <math.h>
#include "mpi.h"
#include <cstdlib>
#include <string>

/* This example handles a 12 x 12 mesh, on 4 processors only. */
#define maxn 12

int main(int argc, char ** argv )
{
    int rank, value, size, errcnt, toterr, i, j, itcnt;
    int i_first, i_last;
    MPI_Status status;
    MPI_File fh;
    MPI_Offset my_offset,my_current_offset;
    double diffnorm, gdiffnorm, norm;
    double xlocal[(12/4)+2][12];
    double xnew[(12/4)+2][12];

    // Prepare transport
    MPI_Init( &argc, &argv );
    uint32_t caps	= 0;
    int ret = cci_init(CCI_ABI_VERSION, 0, &caps);
    if (ret)
    {
        fprintf(stderr, "cci_init() failed with %s\n", cci_strerror(NULL, (cci_status)ret));
        exit(EXIT_FAILURE);
    }

    // Load workflow that describes job network
    Workflow workflow;
    Workflow::make_wflow_from_json(workflow, "linear2.json");

    // Prepare Decaf
    Decaf* decaf = new Decaf(MPI_COMM_WORLD, workflow);

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    if (size != 4) MPI_Abort( MPI_COMM_WORLD, 1 );

    /* xlocal[][0] is lower ghostpoints, xlocal[][maxn+2] is upper */

    /* Note that top and bottom processes have one less row of interior
       points */
    i_first = 1;
    i_last = maxn/size;
    if (rank == 0) i_first++;
    if (rank == size - 1) i_last--;

    /* Fill the data as specified */
    for (i=1; i<=maxn/size; i++)
        for (j=0; j<maxn; j++)
	           xlocal[i][j] = rank;

    for (j=0; j<maxn; j++) {
        xlocal[i_first-1][j] = -1;
        xlocal[i_last+1][j] = -1;
    }

    itcnt = 0;
    do {
        /* Send up unless I'm at the top, then receive from below */
        /* Note the use of xlocal[i] for &xlocal[i][0] */
        if (rank < size - 1)
            MPI_Send( xlocal[maxn/size], maxn, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD );
        if (rank > 0)
            MPI_Recv( xlocal[0], maxn, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &status );

        /* Send down unless I'm at the bottom */
        if (rank > 0)
            MPI_Send( xlocal[1], maxn, MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD );
        if (rank < size - 1)
            MPI_Recv( xlocal[maxn/size+1], maxn, MPI_DOUBLE, rank + 1, 1, MPI_COMM_WORLD, &status );

        /* Compute new values (but not on boundary) */
        itcnt ++;
        diffnorm = 0.0;
        for (i=i_first; i<=i_last; i++)
        for (j=1; j<maxn-1; j++) {
            xnew[i][j] = (xlocal[i][j+1] + xlocal[i][j-1] + xlocal[i+1][j] + xlocal[i-1][j]) / 4.0;
            diffnorm += (xnew[i][j] - xlocal[i][j]) * (xnew[i][j] - xlocal[i][j]);
        }

        /* Only transfer the interior points */
        for (i=i_first; i<=i_last; i++)
            for (j=1; j<maxn-1; j++)
                xlocal[i][j] = xnew[i][j];

        MPI_Allreduce( &diffnorm, &gdiffnorm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD );
        gdiffnorm = sqrt( gdiffnorm );
        if (rank == 0)
            printf( "At iteration %d, diff is %e\n", itcnt, gdiffnorm );
    } while (gdiffnorm > 1.0e-2 && itcnt < 100);

    // Prepare data in Decaf container
    pConstructData container;
    std::shared_ptr<ArrayConstructData<double> > arrayData =
            std::make_shared<ArrayConstructData<double> >(
                &xlocal[1][0], 3*12, 12, false);
    container->appendData("array", arrayData,
                          DECAF_NOFLAG, DECAF_PRIVATE,
                          DECAF_SPLIT_DEFAULT, DECAF_MERGE_APPEND_VALUES);

    // Send the data on all outbound dataflows
    decaf->put(container);

    // Terminate the producer
    fprintf(stderr, "producer %d terminating\n", decaf->world->rank());
    decaf->terminate();

    // Clean Decaf
    delete decaf;

    MPI_Finalize( );
    return 0;
}