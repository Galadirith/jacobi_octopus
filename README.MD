# Jacobi Octopus example

A simple “Hello World” type example for an Octopus workflow modified to run
using [Swift](http://swift-lang.org/docs/index.php).

You will need Java and Perl installed to run Swift workflows. To install Swift

```
$ cd <swift-install-location>
$ wget http://swift-lang.org/packages/swift-0.96.2.tar.gz
$ tar -xzf swift-0.96.2.tar.gz
$ export PATH $PATH:<swift-install-location>/swift-0.96.2/bin
```

## Building

Update the `Makefile` to you your mpi compiler and run

```
make
```

## Usage

To run this example update `workflow.swift` replacing `mpiexec` with your mpi
execution command. Then run the Swift workflow script with

```
swift workflow.swift
```

The end result will be three files. `s.out` and `a.out` contain the standard
output from the two programs `jacobi` and `jacobi_analyse`. `x.mat` is the
file output by `jacobi` and consumed by `jacobi_analyse`.

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
