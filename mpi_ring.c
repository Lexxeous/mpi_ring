// include necessary libraries
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// definitions
#define MAX_STR_LEN 100 // maximum string length
#define SR_MSG_TAG 108 // send/recv message tag ; MAX_STR_LEN + MAX_WLD_SIZE
#define SR_VAL_TAG 10 // send/recv value tag
#define VAL_CNT 10 // value count ; abs(int) = 2,147,483,684
#define MIN_WLD_SIZE 3 // minimum <world_size>
#define MAX_WLD_SIZE 8 // maximum <world_size>

int main(int argc, char* argv[])
{
  // initialize the MPI environment
  MPI_Init(NULL, NULL);
  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size); // get the number of processes
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); // get the ranks

  // command line arguments must be the program, the string, and the integer
  if (argc != 3 && world_rank == 0)
  {
    printf("ERR:3:ARGC >> Wrong number of command line arguments.\nUse ./<executable> <string> <integer> as format.\n");
    return -3;
  }

  // only allow 3 ~ 8 processes for the ring
  if ((world_size < MIN_WLD_SIZE || world_size > MAX_WLD_SIZE) && world_rank == 0)
  {
    fprintf(stderr, "ERR:%d:WRLDSZ >> Number of processes must be between 3 and 10 for %s.\n", world_size, argv[0]);
    MPI_Abort(MPI_COMM_WORLD, world_size);
  }

  // validate the length of input string
  if(strlen(argv[1]) > MAX_STR_LEN && world_rank == 0)
  {
    fprintf(stderr, "ERR:0:STR >> The second command line argument must be a string of size ≤ %d.\n", MAX_STR_LEN);
    MPI_Abort(MPI_COMM_WORLD, 0);
  }

  // initialize the message that will be appended and passed between processes
  char msg[MAX_STR_LEN];
  strcpy(msg, argv[1]);

  // initialalize the integer that will be incremented and passed between processes
  static int val;
  val = atoi(argv[2]);

  // print an intro, the original message, and the value for reference
  if(world_rank == 0)
  {
    printf("\nStarting MPI ring of size %d...\n", world_size);
    printf("Original message: %s\n", msg);
    printf("Original value: %d\n", val);
  }

  // starting MPI ring of size <world_size>
  if (world_rank != 0) // master process (process 0) skips the "if" and sends out message and value first
  {
    int prev_rank = world_rank - 1; // calculate the previous rank

    // worker processes receive from the previous rank
    MPI_Recv(&msg, MAX_STR_LEN + world_size, MPI_CHAR, prev_rank, SR_MSG_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&val, VAL_CNT, MPI_INT, prev_rank, SR_VAL_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // append rank to the end of the messasge
    char worker_rank[2];
    sprintf(worker_rank, "%d", world_rank);
    strcat(msg, worker_rank);

    // print worker(s) result
    printf("\nMessage at process %d: %s\n", world_rank, msg);
    printf("Value at process %d: %d\n", world_rank, val);
  }

  val = val + 1; // every process needs to increment the value

  int next_rank = (world_rank + 1) % world_size; //calculate the next rank

  // send to next rank
  MPI_Send(&msg, MAX_STR_LEN + world_size, MPI_CHAR, next_rank, SR_MSG_TAG, MPI_COMM_WORLD);
  MPI_Send(&val, VAL_CNT, MPI_INT, next_rank, SR_VAL_TAG, MPI_COMM_WORLD);

  // master process (process 0) is the last process to receive the string and the value
  if (world_rank == 0)
  {
    int last_rank = world_size - 1; // calculate the last rank

    // master process (process 0) receives from the last rank
    MPI_Recv(&msg, MAX_STR_LEN + world_size, MPI_CHAR, last_rank, SR_MSG_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&val, VAL_CNT, MPI_INT, last_rank, SR_VAL_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // append rank to the end of the messasge
    char master_rank[2];
    sprintf(master_rank, "%d", world_rank);
    strcat(msg, master_rank);

    // print master result
    printf("\nMessage at process %d: %s\n", world_rank, msg);
    printf("Value at process %d: %d\n", world_rank, val);
  }

  // Finalize the MPI environment.
  MPI_Finalize();
}

/*
Function formats for "MPI_Send()" and "MPI_Recv()":
  MPI_Send(void* data, int count, MPI_Datatype datatype, int destination, int tag, MPI_Comm communicator)
  MPI_Recv(void* data, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm communicator, MPI_Status* status)
*/

/*
  ########################################################################################

  RESULT WITH 8 PROCESSES:

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

  RESULT WITH 5 PROCESSES:

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

  RESULT WITH 3 PROCESSES:

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

  RESULT WITH 9 PROCESSES:

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

  RESULT WITH 2 PROCESSES:

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
  ERR:3:ARGC >> Wrong number of command line arguments.
  Use ./<executable> <string> <integer> as format.
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
