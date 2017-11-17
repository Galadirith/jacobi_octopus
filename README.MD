# Jacobi Octopus example

A simple “Hello World” type example for an Octopus workflow modified to

## Building

This example assumes you already have an installation of Decaf available and
an installation of CCI.

  ```
  export DECAF_PREFIX=<path-to-decaf-install>
  export LD_LIBRARY_PATH=<path-to-decaf-install>/lib:$LD_LIBRARY_PATH
  export PATH=<path-to-boost-install>/include:$PATH
  export LD_LIBRARY_PATH=<path-to-boost-install>/lib:$LD_LIBRARY_PATH
  export LD_LIBRARY_PATH=<path-to-cci-install>/lib:$LD_LIBRARY_PATH
  cmake -DDECAF_PREFIX=${DECAF_PREFIX} -DCCI_PREFIX=<path-to-cci-install> .
  make
  ```

update the Makefile for compiler and mpi command in the Jacobi_wf.sh script.

## Usage

To run this example, you need to generate the workflow configuration using the
appropriate python script. This will also generate a bash script that can be
used to execute the workflow on most systems.

  ```
  export DECAF_PREFIX=<path-to-decaf-install>
  export LD_LIBRARY_PATH=<path-to-decaf-install>/lib:$LD_LIBRARY_PATH
  export LD_LIBRARY_PATH=<path-to-boost-install>/lib:$LD_LIBRARY_PATH
  export LD_LIBRARY_PATH=<path-to-cci-install>/lib:$LD_LIBRARY_PATH
  python workflow.py
  ./linear2.sh
  ```

## Notes

- **Manager reguirement**  
  - S = jacobi  
  - A = jacobi_analyse  
  - dependency = x.mat file

- **CDO requirement**  
  - S
    - xlocal[12][12] partitioned across 4 ranks as xlocal[1:3][0:11]  
    - (includes halos, xlocal[0][0:11] & xlocal[4][0:11])  
    - xlocal decomposed in x direction only.  
  - A
    - xlocal[12][12] partioned across 4 ranks as xlocal[0:2][0:11]  
  - CDO^S == CDO^A  

- **Transport requirement**  
  S and A to run a separate mpi process (or a single mpi process). A to start after a delay.
