/**
 * Removed all code related to MPI, does not create any workers or master, simply runs sequentially...
*/

#include <stdio.h>
#include <stdlib.h>

void srandom (unsigned seed);
double dboard (int darts);
#define DARTS 50000     /* number of throws at dartboard */
#define ROUNDS 100      /* number of times "darts" is iterated */

int main (int argc, char *argv[])
{
   double	homepi,         /* value of pi calculated by current task */
            pisum,	        /* sum of tasks' pi values */
            pi,	        /* average of pi after "darts" is thrown */
            avepi;	        /* average pi value for all iterations */
   int	   taskid,	        /* task ID - also used as seed number */
            numtasks,       /* number of tasks */
            rc,             /* return code */
            i;

   /* Set seed for random number generator equal to 1 since there is only one task */
   srandom (1);

   avepi = 0;
   for (i = 0; i < ROUNDS; i++) {
      /* All tasks calculate pi using dartboard algorithm */
      homepi = dboard(DARTS);
      avepi = ((avepi * i) + homepi)/(i + 1); 
      printf("   After %8d throws, average value of pi = %10.8f\n",
               (DARTS * (i + 1)),avepi);
         
   } 
   printf ("\nReal value of PI: 3.1415926535897 \n");

   return 0;
}

double dboard(int darts)
{
   #define sqr(x)	((x)*(x))
   long random(void);
   double x_coord, y_coord, pi, r; 
   int score, n;
   unsigned int cconst;  /* must be 4-bytes in size */
   /*************************************************************************
    * The cconst variable must be 4 bytes. We check this and bail if it is
    * not the right size
    ************************************************************************/
   if (sizeof(cconst) != 4) {
      printf("Wrong data size for cconst variable in dboard routine!\n");
      printf("See comments in source file. Quitting.\n");
      exit(1);
   }
   /* 2 bit shifted to MAX_RAND later used to scale random number between 0 and 1 */
   cconst = 2 << (31 - 1);
   score = 0;

   /* "throw darts at board" */
   for (n = 1; n <= darts; n++)  {
      /* generate random numbers for x and y coordinates */
      r = (double)random()/cconst;
      x_coord = (2.0 * r) - 1.0;
      r = (double)random()/cconst;
      y_coord = (2.0 * r) - 1.0;

      /* if dart lands in circle, increment score */
      if ((sqr(x_coord) + sqr(y_coord)) <= 1.0)
           score++;
      }

   /* calculate pi */
   pi = 4.0 * (double)score/(double)darts;
   return(pi);
} 
