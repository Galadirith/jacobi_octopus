type file;

app (file o, file dep) simulation ()
{
  mpiexec "--allow-run-as-root" "-n" 4 "/root/projects/jacobi_octopus/jacobi" stdout=filename(o);
}

app (file o) analyze (file dep)
{
  mpiexec "--allow-run-as-root" "-n" 4 "/root/projects/jacobi_octopus/jacobi_analyse" stdout=filename(o);
}


file sStdout <"s.out">;
file aStdout <"a.out">;
file xmat <"x.mat">;

(sStdout, xmat) = simulation();
(aStdout) = analyze(xmat);
