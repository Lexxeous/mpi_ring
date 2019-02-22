1.
Run "hpcshell --tasks-per-node=<TPN>" to reserve a certain number of processors.

2.
Modify "Makefile" to reflect the quanity of <TPN> and assign the input string and value for the command line arguments.

3.
Run "make comp" to compile the "mpi_ring.c" into an executable called "mpi_ring".

4.
Run "make run" to run the executable.

5.
Run "make clean" to remove the executable from the current directory.


/*
  ########################################################################################

  RESULT WITH 8 NODES:

  Execution Commands:
  >> hpcshell --tasks-per-node=8
  >> make comp
    mpicc -o mpi_ring mpi_ring.c
  >> make run
    mpirun -np 8 ./mpi_ring testing_ring 1

  Output:
  Starting MPI ring of size 8...
  Original message: testing_ring
  Original value: 8675309

  Message at process 1: testing_ring1
  Value at process 1: 8675310

  Message at process 2: testing_ring12
  Value at process 2: 8675311

  Message at process 3: testing_ring123
  Value at process 3: 8675312

  Message at process 4: testing_ring1234
  Value at process 4: 8675313

  Message at process 5: testing_ring12345
  Value at process 5: 8675314

  Message at process 6: testing_ring123456
  Value at process 6: 8675315

  Message at process 7: testing_ring1234567
  Value at process 7: 8675316

  Message at process 0: testing_ring12345670
  Value at process 0: 8675317

  ########################################################################################

  RESULT WITH 5 NODES:

  Execution Commands:
  >> hpcshell --tasks-per-node=5
  >> make comp
    mpicc -o mpi_ring mpi_ring.c
  >> make run
    mpirun -np 5 ./mpi_ring ring 5000

  Starting MPI ring of size 5...
  Original message: ring
  Original value: 5000

  Message at process 1: ring1
  Value at process 1: 5001

  Message at process 2: ring12
  Value at process 2: 5002

  Message at process 3: ring123
  Value at process 3: 5003

  Message at process 4: ring1234
  Value at process 4: 5004

  Message at process 0: ring12340
  Value at process 0: 5005

  ########################################################################################

  RESULT WITH 3 NODES:

  Execution Commands:
  >> hpcshell --tasks-per-node=3
  >> make comp
    mpicc -o mpi_ring mpi_ring.c
  >> make run
    mpirun -np 3 ./mpi_ring string 80

  Starting MPI ring of size 3...
  Original message: string
  Original value: 80

  Message at process 1: string1
  Value at process 1: 81

  Message at process 2: string12
  Value at process 2: 82

  Message at process 0: string120
  Value at process 0: 83

  ########################################################################################

  RESULT WITH 9 NODES:

  Execution Commands:
  >> hpcshell --tasks-per-node=9
  >> make comp
    mpicc -o mpi_ring mpi_ring.c
  >> make run
    mpirun -np 9 ./mpi_ring string 80

  Output:
  ERR:9:WRLDSZ >> Number of processes must be between 3 and 8 for ./mpi_ring.
  --------------------------------------------------------------------------
  MPI_ABORT was invoked on rank 0 in communicator MPI_COMM_WORLD
  with errorcode 9.

  NOTE: invoking MPI_ABORT causes Open MPI to kill all MPI processes.
  You may or may not see output from other processes, depending on
  exactly when Open MPI kills them.
  --------------------------------------------------------------------------
  make: *** [run] Error 9

  ########################################################################################

  RESULT WITH 2 NODES:

  Execution Commands:
  >> hpcshell --tasks-per-node=2
  >> make comp
    mpicc -o mpi_ring mpi_ring.c
  >> make run
    mpirun -np 2 ./mpi_ring string 80

  ERR:2:WRLDSZ >> Number of processes must be between 3 and 8 for ./mpi_ring.
  --------------------------------------------------------------------------
  MPI_ABORT was invoked on rank 0 in communicator MPI_COMM_WORLD
  with errorcode 2.

  NOTE: invoking MPI_ABORT causes Open MPI to kill all MPI processes.
  You may or may not see output from other processes, depending on
  exactly when Open MPI kills them.
  --------------------------------------------------------------------------
  make: *** [run] Error 2

  ########################################################################################

  RESULT WITH TOO MANY COMMAND LINE ARGUMENTS:

  Execution Commands:
  >> hpcshell --tasks-per-node=5
  >> make comp
    mpicc -o mpi_ring mpi_ring.c
  >> make run
    mpirun -np 5 ./mpi_ring ring 100 test
  ERR:3:ARGC >> Wrong number of command line arguments. Use ./<executable> <string> <integer> as format.
  -------------------------------------------------------
  Primary job  terminated normally, but 1 process returned
  a non-zero exit code.. Per user-direction, the job has been aborted.
  -------------------------------------------------------
  --------------------------------------------------------------------------
  mpirun detected that one or more processes exited with non-zero status, thus causing
  the job to be terminated. The first process to do so was:

    Process name: [[17468,1],0]
    Exit code:    253
  --------------------------------------------------------------------------

  ########################################################################################

  RESULT WITH AN EXCEEDINGLY LONG INPUT STRING:

  Execution Commands:
  >> hpcshell --tasks-per-node=7
  >> make comp
    mpicc -o mpi_ring mpi_ring.c
  >> make run
    mpirun -np 7 ./mpi_ring s0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789 1
  ERR:0:STR >> The second command line argument must be a string of size ≤ 100.
  --------------------------------------------------------------------------
  MPI_ABORT was invoked on rank 0 in communicator MPI_COMM_WORLD
  with errorcode 0.

  NOTE: invoking MPI_ABORT causes Open MPI to kill all MPI processes.
  You may or may not see output from other processes, depending on
  exactly when Open MPI kills them.
  --------------------------------------------------------------------------
  make: *** [run] Error 0

  ########################################################################################
*/
