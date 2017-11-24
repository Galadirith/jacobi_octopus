#ifndef JACOBI_OCTOPUS_API_H
#define JACOBI_OCTOPUS_API_H

#include <decaf/decaf.hpp>
#include <bredala/data_model/arrayconstructdata.hpp>
#include <stdio.h>
#include <math.h>
#include "mpi.h"
#include <cstdlib>
#include <string>

class Octopus
{
public:
    Octopus();
    ~Octopus();
    void putArray(double *data, int size, int stride);
    int getArray(double **data);

private:
    static uint32_t caps;
    static int ret;
    Workflow workflow;
    Decaf* decaf;
    std::shared_ptr<ArrayConstructData<double> > arrayData;
};

#endif /* JACOBI_OCTOPUS_API_H */
