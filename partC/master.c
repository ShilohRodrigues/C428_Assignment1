#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define DARTS 50000     /* number of throws at dartboard */
#define ROUNDS 100      /* number of times "darts" is iterated */
#define WORKER_PROGRAM "./slave"

int main(int argc, char *argv[]) {

  double	homepi, pisum, pi, avepi = 0;	        
  int	taskid, numtasks, rc, i, numWorkers;     

  MPI_Comm workerComm; //Communicator for worker tasks

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD,&taskid);

  if ( argc != 2 )
      printf( "usage: %s <number of workers>\n", argv[0] );
  else
      numWorkers = atoi( argv[1] );

  MPI_Comm_spawn(WORKER_PROGRAM, argv, numWorkers, MPI_INFO_NULL, 0, MPI_COMM_SELF, &workerComm, MPI_ERRCODES_IGNORE );
  
  for (i = 0; i < ROUNDS; i++) {
    MPI_Reduce(NULL, &pisum, 1, MPI_DOUBLE, MPI_SUM, MPI_ROOT, workerComm);
    pi = pisum / numWorkers;
    avepi = ((avepi * i) + pi) / (i + 1);
    printf("After %8d throws, average value of pi = %10.8f\n", DARTS * (i + 1), avepi);
  }

  printf ("\nReal value of PI: 3.1415926535897 \n");

  MPI_Finalize();
  return 0;

}