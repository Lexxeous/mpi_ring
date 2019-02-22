// include necessary libraries
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// definitions
#define MAX_STR_LEN 100 // maximum string length
#define SR_MSG_TAG 103 // send/recv message tag ; MAX_STR_LEN + world_size
#define SR_VAL_TAG 10 // send/recv value tag
#define VAL_CNT 10 // value count ; abs(int) = 2,147,483,684
#define MIN_WLD_SIZE 3 // minimum <world_size>
#define MAX_WLD_SIZE 10 // maximum <world_size>

int main(int argc, char* argv[])
{
  // command line arguments must be the program, the string, and the integer
  if (argc != 3)
  {
    printf("ERR:3:ARGC >> Wrong number of command line arguments. Use ./<executable> <string> <integer> as format.\n");
    return -3;
  }

  // initialize the MPI environment
  MPI_Init(NULL, NULL);
  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size); // get the number of processes
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); // get the ranks

  // only allow 3 ~ 10 processes for the ring
  if (world_size < MIN_WLD_SIZE || world_size > MAX_WLD_SIZE)
  {
    fprintf(stderr, "ERR:%d:WRLDSZ >> Number of processes must be between 3 and 10 for %s.\n", world_size, argv[0]);
    MPI_Abort(MPI_COMM_WORLD, world_size);
  }

  // validate the length of input string
  if(strlen(argv[1]) > MAX_STR_LEN && world_rank == 0)
  {
    fprintf(stderr, "ERR:1:STR >> The first input must be a string of size ≤ %d.\n", MAX_STR_LEN);
    MPI_Abort(MPI_COMM_WORLD, 1);
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
