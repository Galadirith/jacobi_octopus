#include "api.hpp"
#include <bredala/data_model/pconstructtype.h>
#include <bredala/data_model/arrayconstructdata.hpp>
#include <cci.h>
#include <bredala/data_model/boost_macros.h>

#include <stdio.h>
#include <math.h>
#include "mpi.h"
#include <cstdlib>
#include <string>

uint32_t Octopus::caps = 0;
int Octopus::ret = 0;

Octopus::Octopus() {
    // Initialize CCI transport
    caps = 0;
    ret = cci_init(CCI_ABI_VERSION, 0, &caps);
    if (ret)
    {
        fprintf(
            stderr, "cci_init() failed with %s\n",
            cci_strerror(NULL, (cci_status)ret));
        exit(EXIT_FAILURE);
    }

    // Load workflow that describes job network
    Workflow::make_wflow_from_json(workflow, "linear2.json");

    // Prepare Decaf
    decaf = new Decaf(MPI_COMM_WORLD, workflow);
}

Octopus::~Octopus() {
    // Terminate the producer
    fprintf(stderr, "producer %d terminating\n", decaf->world->rank());
    decaf->terminate();

    // Clean Decaf
    delete decaf;
}

void Octopus::putArray(double *data, int size, int stride) {
    // Prepare data in Decaf container and send
    pConstructData container;
    std::shared_ptr<ArrayConstructData<double> > arrayData =
        std::make_shared<ArrayConstructData<double> >(
            data, size, stride, false);
    container->appendData(
        "array", arrayData,
        DECAF_NOFLAG, DECAF_PRIVATE,
        DECAF_SPLIT_DEFAULT, DECAF_MERGE_APPEND_VALUES);
    decaf->put(container);
}

int Octopus::getArray(double **data) {
    vector< pConstructData > in_data;
    if (decaf->get(in_data)) {
        arrayData =
            in_data[0]->getTypedData<ArrayConstructData<double> >("array");
        *data = arrayData->getArray();
        return 0;
    } else {
        return 1;
    }
}
