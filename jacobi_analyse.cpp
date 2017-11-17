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

    vector< pConstructData > in_data;
    while (decaf->get(in_data))
    {
        fprintf(stderr, "size in_data: %lu\n", in_data.size());
        for (i = 0; i < in_data.size(); i++)
        {
            std::shared_ptr<ArrayConstructData<double> > arrayData =
                in_data[i]->getTypedData<ArrayConstructData<double> >("array");
            double* xlocal = arrayData->getArray();

            // Print array head for debug
            std::string row;
            for (i=0; i<3; i++)
            {
                row = "";
                for (j=0; j<12; j++)
                  row += std::to_string(xlocal[i*12 + j]) + ", ";
                fprintf(stderr, "rank %d: %s\n", rank, row.c_str());
            }

            fprintf(stderr, "size xlocal: %d\n", arrayData->getSize());
            for(j=0;j<36;j++)
                norm+= xlocal[j]*xlocal[j];
        }
    }

    MPI_Allreduce( &norm, &totalnorm, 1, MPI_DOUBLE, MPI_SUM,MPI_COMM_WORLD );

    printf("rank %d : %f\n",rank,totalnorm);

    // Terminate the consumer
    fprintf(stderr, "consumer %d terminating\n", decaf->world->rank());
    decaf->terminate();

    // Clean Decaf
    delete decaf;

    MPI_Finalize();

}
