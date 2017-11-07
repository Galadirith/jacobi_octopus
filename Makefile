# Generated automatically from Makefile.in by configure.
ALL: jacobi jacobi_analyse
SHELL = /bin/sh
DIRS = 
jacobi: jacobi.c
	CC -o jacobi jacobi.c -lm 
jacobi_analyse: jacobi_analyse.c
	CC -o jacobi_analyse jacobi_analyse.c -lm 
clean:
	/bin/rm -f jacobi jacobi.o jacobi.log jacobi_analyse.o jacobi_analyse
