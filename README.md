# mpi_ring

A simple program that creates a ring of communicative processing nodes using [C](https://en.wikipedia.org/wiki/C_(programming_language)) and [MPI](https://en.wikipedia.org/wiki/Message_Passing_Interface).

This parallel processing program that creates a ring of 7 nodes (0 ~ 6).

The first command line argument specifies an input string of length ≤ 100.
The second command line argument specifies an numeric input.

Processes 0 through 6 will pass the string and the value to thier next neighbors after appending thier rank and incrementing the value.
Any extra processes defined in the "mpirun" are unused.

## Result with 7 Nodes:

Execution Commands:
```sh
hpcshell --tasks-per-node=7
make comp
mpicc -o mpi_ring mpi_ring.c
make run
mpirun -np 7 ./mpi_ring hello 70
```

Output:
```
Original message: hello
Original value: 70

Message at process 1: hello1
Value at process 1: 71

Message at process 2: hello12
Value at process 2: 72

Message at process 3: hello123
Value at process 3: 73

Message at process 4: hello1234
Value at process 4: 74

Message at process 5: hello12345
Value at process 5: 75

Message at process 6: hello123456
Value at process 6: 76

Message at process 0: hello1234560
Value at process 0: 77
```

## Result with 9 Nodes:

Execution Commands:
```sh
hpcshell --tasks-per-node=9
make comp
mpicc -o mpi_ring mpi_ring.c
make run
mpirun -np 9 ./mpi_ring string 90
```

Output:
```
Process 7 is unused.

Process 8 is unused.

Original message: string
Original value: 90

Message at process 1: string1
Value at process 1: 91

Message at process 2: string12
Value at process 2: 92

Message at process 3: string123
Value at process 3: 93

Message at process 4: string1234
Value at process 4: 94

Message at process 5: string12345
Value at process 5: 95

Message at process 6: string123456
Value at process 6: 96

Message at process 0: string1234560
Value at process 0: 97
```
