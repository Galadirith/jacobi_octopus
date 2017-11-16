# Generated automatically from Makefile.in by configure.
ALL: jacobi jacobi_analyse
SHELL = /bin/sh
DIRS =
jacobi: jacobi.cpp
	CC -o jacobi jacobi.cpp -lm
jacobi_analyse: jacobi_analyse.cpp
	CC -o jacobi_analyse jacobi_analyse.cpp -lm
clean:
	/bin/rm -f jacobi jacobi.o jacobi.log jacobi_analyse.o jacobi_analyse
