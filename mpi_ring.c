//include necessary libraries
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// definitions
#define MAX_STR_LEN 100 // maximum string length
#define SR_MSG_TAG 103 // send/recv message tag
#define SR_VAL_TAG 50 // send/recv value tag
#define VAL_CNT 50 // value count
#define MIN_WLD_SIZE 3 // minimum <world_size>
#define MAX_WLD_SIZE 10 // maximum <world_size>

// prototyping function(s)
void append_char(char* s, char c);

// start main
int main(int argc, char* argv[])
{
  // initialize the MPI environment
  MPI_Init(NULL, NULL);

  // get the number of processes
  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size); // total number of processes
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); // rank of processes starting from 0 ~ (world_size - 1)

  // validate the length of input string
  if(strlen(argv[1]) > MAX_STR_LEN && world_rank == 0)
  {
    fprintf(stderr, "The first input must be a string of size ≤ %d.\n", MAX_STR_LEN);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  // initialize the message that will be appended and passed between processes
  char msg[MAX_STR_LEN];
  strcpy(msg, argv[1]);

  int val = atoi(argv[2]); // the integer that will be incremented and passed between processes

  // assuming 3 ~ 10 processes for the ring
  if (world_size < 3 && world_rank == 0)
  {
    fprintf(stderr, "MPI <world_size> must be greater than or equal to %d for %s\n", MIN_WLD_SIZE, argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 3);
  }
  else if (world_size > 10 && world_rank == 0)
  {
    fprintf(stderr, "MPI <world_size> must be less than or equal to %d for %s\n", MAX_WLD_SIZE, argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 10);
  }

  if(world_rank == 0)
  {
    printf("\nOriginal message: %s\n", msg);
    printf("Original value: %d\n", val);
  }

  // starting MPI ring of size <world_size>
  if(world_rank == 0) // process 0
  {
    // send original <msg> and <val> to process 1
    MPI_Send(&msg, MAX_STR_LEN + world_size, MPI_CHAR, 1, SR_MSG_TAG, MPI_COMM_WORLD); // send <msg> to process 1
    MPI_Send(&val, VAL_CNT, MPI_INT, 1, SR_VAL_TAG, MPI_COMM_WORLD); // send <val> to process 1

    // receive <msg> and <val> from process 6
    MPI_Recv(&msg, MAX_STR_LEN + world_size, MPI_CHAR, 6, SR_MSG_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv <msg> from process 6
    MPI_Recv(&val, VAL_CNT, MPI_INT, 6, SR_VAL_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv <val> from process 6

    // append and increment
    append_char(msg, '0'); // append rank 0
    val = val + 1; // increment the integer

    // print result
    printf("\nMessage at process 0: %s\n", msg);
    printf("Value at process 0: %d\n", val);
  }
  else if(world_rank == 1) // process 1
  {
    // receive <msg> and <val> from process 0
    MPI_Recv(&msg, MAX_STR_LEN + world_size, MPI_CHAR, 0, SR_MSG_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv <msg> from process 0
    MPI_Recv(&val, VAL_CNT, MPI_INT, 0, SR_VAL_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv <val> from process 0

    // append and increment
    append_char(msg, '1'); // append rank 1
    val = val + 1; // increment the integer

    // print result
    printf("\nMessage at process 1: %s\n", msg);
    printf("Value at process 1: %d\n", val);

    // send modified <msg> and <val> to process 2
    MPI_Send(&msg, MAX_STR_LEN + world_size, MPI_CHAR, 2, SR_MSG_TAG, MPI_COMM_WORLD); // send <msg> to process 2
    MPI_Send(&val, VAL_CNT, MPI_INT, 2, SR_VAL_TAG, MPI_COMM_WORLD); // send <val> to process 2
  }
  else if(world_rank == 2) // process 2
  {
    // receive <msg> and <val> from process 1
    MPI_Recv(&msg, MAX_STR_LEN + world_size, MPI_CHAR, 1, SR_MSG_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv <msg> from process 1
    MPI_Recv(&val, VAL_CNT, MPI_INT, 1, SR_VAL_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv <val> from process 1

    // append and increment
    append_char(msg, '2'); // append rank 2
    val = val + 1; // increment the integer

    // print result
    printf("\nMessage at process 2: %s\n", msg);
    printf("Value at process 2: %d\n", val);

    // send modified <msg> and <val> to process 3
    MPI_Send(&msg, MAX_STR_LEN + world_size, MPI_CHAR, 3, SR_MSG_TAG, MPI_COMM_WORLD); // send <msg> to process 3
    MPI_Send(&val, VAL_CNT, MPI_INT, 3, SR_VAL_TAG, MPI_COMM_WORLD); // send <val> to process 3
  }
  else if(world_rank == 3) // process 3
  {
    // receive <msg> and <val> from process 2
    MPI_Recv(&msg, MAX_STR_LEN + world_size, MPI_CHAR, 2, SR_MSG_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv <msg> from process 2
    MPI_Recv(&val, VAL_CNT, MPI_INT, 2, SR_VAL_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv <val> from process 2

    // append and increment
    append_char(msg, '3'); // append rank 3
    val = val + 1; // increment the integer

    // print result
    printf("\nMessage at process 3: %s\n", msg);
    printf("Value at process 3: %d\n", val);

    // send modified <msg> and <val> to process 4
    MPI_Send(&msg, MAX_STR_LEN + world_size, MPI_CHAR, 4, SR_MSG_TAG, MPI_COMM_WORLD); // send <msg> to process 4
    MPI_Send(&val, VAL_CNT, MPI_INT, 4, SR_VAL_TAG, MPI_COMM_WORLD); // send <val> to process 4
  }
  else if(world_rank == 4) // process 4
  {
    // receive <msg> and <val> from process 3
    MPI_Recv(&msg, MAX_STR_LEN + world_size, MPI_CHAR, 3, SR_MSG_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv <msg> from process 3
    MPI_Recv(&val, VAL_CNT, MPI_INT, 3, SR_VAL_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv <val> from process 3

    // append and increment
    append_char(msg, '4'); // append rank 4
    val = val + 1; // increment the integer

    // print result
    printf("\nMessage at process 4: %s\n", msg);
    printf("Value at process 4: %d\n", val);

    // send modified <msg> and <val> to process 5
    MPI_Send(&msg, MAX_STR_LEN + world_size, MPI_CHAR, 5, SR_MSG_TAG, MPI_COMM_WORLD); // send <msg> to process 5
    MPI_Send(&val, VAL_CNT, MPI_INT, 5, SR_VAL_TAG, MPI_COMM_WORLD); // send <val> to process 5
  }
  else if(world_rank == 5) // process 5
  {
    // receive <msg> and <val> from process 4
    MPI_Recv(&msg, MAX_STR_LEN + world_size, MPI_CHAR, 4, SR_MSG_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv <msg> from process 4
    MPI_Recv(&val, VAL_CNT, MPI_INT, 4, SR_VAL_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv <val> from process 4

    // append and increment
    append_char(msg, '5'); // append rank 5
    val = val + 1; // increment the integer

    // print result
    printf("\nMessage at process 5: %s\n", msg);
    printf("Value at process 5: %d\n", val);

    // send modified <msg> and <val> to process 6
    MPI_Send(&msg, MAX_STR_LEN + world_size, MPI_CHAR, 6, SR_MSG_TAG, MPI_COMM_WORLD); // send <msg> to process 6
    MPI_Send(&val, VAL_CNT, MPI_INT, 6, SR_VAL_TAG, MPI_COMM_WORLD); // send <val> to process 6
  }
  else if(world_rank == 6) // process 6
  {
    // receive <msg> and <val> from process 5
    MPI_Recv(&msg, MAX_STR_LEN + world_size, MPI_CHAR, 5, SR_MSG_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv <msg> from process 5
    MPI_Recv(&val, VAL_CNT, MPI_INT, 5, SR_VAL_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recv <val> from process 5

    // append and increment
    append_char(msg, '6'); // append rank 6
    val = val + 1; // increment the integer

    // print result
    printf("\nMessage at process 6: %s\n", msg);
    printf("Value at process 6: %d\n", val);

    // send modified <msg> and <val> to process 0
    MPI_Send(&msg, MAX_STR_LEN + world_size, MPI_CHAR, 0, SR_MSG_TAG, MPI_COMM_WORLD); // send <msg> to process 0
    MPI_Send(&val, VAL_CNT, MPI_INT, 0, SR_VAL_TAG, MPI_COMM_WORLD); // send <val> to process 0
  }
  else 
  {
    printf("\nProcess %d is unused.\n", world_rank); // extra processes
  }

  // finalize the MPI environment.
  MPI_Finalize();
}

// implementing prototyped function(s)
void append_char(char* s, char c)
{
  int len = strlen(s); // get the string length
  s[len] = c; // place the character at the end of the string
  s[len+1] = '\0'; // assigned the next place in the string to be a null terminator
}

// function format of MPI_Send and MPI_Recv
// MPI_Send(void* data, int count, MPI_Datatype datatype, int destination, int tag, MPI_Comm communicator)
// MPI_Recv(void* data, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm communicator, MPI_Status* status)

/*
  Execution Commands:
  >> hpcshell --tasks-per-node=7
  >> make comp
    mpicc -o mpi_ring mpi_ring.c
  >> make run
    mpirun -np 7 ./mpi_ring ring 70

  Output:
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

  ––––––––––––––––––––––––––––––––––––––––––

  Execution Commands:
  >> hpcshell --tasks-per-node=9
  >> make comp
    mpicc -o mpi_ring mpi_ring.c
  >> make run
    mpirun -np 9 ./mpi_ring string 90

  Output:
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
*/
