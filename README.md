# mpi_ring

A simple program that creates a ring of communicative processing nodes using [C](https://en.wikipedia.org/wiki/C_(programming_language)) and [MPI](https://en.wikipedia.org/wiki/Message_Passing_Interface).

This parallel processing program that creates a ring of 7 nodes (0 ~ 6).

The first command line argument specifies an input string of length ≤ 100.
The second command line argument specifies an numeric input.

Processes 0 through 6 will pass the string and the value to thier next neighbors after appending thier rank and incrementing the value.
Any extra processes defined in the "mpirun" are unused.