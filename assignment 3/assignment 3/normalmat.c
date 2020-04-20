//Heman Baral
//Assignment 3

#include <sys/times.h>
#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>

//Macro for MIN
  #define min(X, Y)  ((X) < (Y) ? (X) : (Y))
  #define blocksize 256

double *sharedA, *sharedB,  *sharedC;
sem_t *sem;
int cn =0 ;



double start, stop, used, mf, *time;

//Time

double ftime(void);
void multiply (double **a, double **b, double **c, int n);

double ftime (void)
{
  struct timeval t;

  gettimeofday(&t, NULL);
  

  //times ( &t );

  return (double) t.tv_sec +(double) t.tv_usec/ 1000000.0;
}







/////display function//////

void MatrixDisplay(double **c, int n)
{


  int a, b;

  for(a = 0; a<n; a++)
  {
   for(b=0; b<n;b++)
   {

    printf("%f ", c[a][b]);

  }

  printf("\n");

}

printf("\n\n");

}   

////////normal multiplication////////

void multiply (double **a, double **b, double **c, int n)
{
 int i, j, k;

 for (i=0; i<n; i++)
 {
   for (j=0; j<n; j++)

     c[i][j] = 0;
 }

 for (i=0; i<n; i++)
 {
   for (j=0; j<n; j++)
   {
     for (k=0; k<n; k++)
       c[i][j]= c[i][j] + a[i][k] * b[k][j];
   }
 }

}


  ///////end normal////////

//////transpose////////// 

void transpose(double **b, int n)
{
 int temp, i, j;
 for (i = 1; i < n; i++) {
  for (j = 0; j < i; j++) {
   temp = b[i][j];
   b[i][j] = b[j][i];
   b[j][i] = temp;
 }
}

}

 ///////end transpose//////////

 ///////transpose multiplication///////

void transmultiply (double **a, double **b, double **c, int n)
{
 int i, j, k;

 for (i=0; i<n; i++)
 {
   for (j=0; j<n; j++)

     c[i][j] = 0;
 }

 for (i=0; i<n; i++)
 {
   for (j=0; j<n; j++)
   {
     for (k=0; k<n; k++)
       c[i][j]= c[i][j] + a[i][k] * b[j][k];
   }
 }

}



////////end transpose multiplication/////////


///////Block//////////////////


void block(double **a, double **b, double **c, int n)
{

  int i0, i, j0, j, k, k0;

  for (i0 = 1; i0<n; i0+=blocksize) {  

    //i0=min(i0+blocksize,n);

    for (j0 = 1; j0<n; j0+=blocksize) {  


      //j0=min(j0+blocksize,n);

      for (k0 = 1; k0<n; k0+=blocksize) {  

        //k0=min(k0+blocksize,n);




        for (i = i0; i<min(i0+blocksize,n);i++) {  


         for (j = j0; j<min(j0+blocksize,n);j++) {  


           for (k = k0; k<min(k0+blocksize,n);k++) {  


             c[i][j] = c[i][j] + a[i][k] * b[k][j] ; 
           }


         } 


       }


     } 


   } 


 }



}

//////End Block////////////////////////



/////////Shared Memory set Up Setup//////////////////

volatile int *mem; 
sem_t *sem;


// void work ( int me )

// {
//     int i, n;
//     for ( i = 0; i < n; i++ ) {
//         sem_wait ( sem );
//         *mem = *mem;
//         sem_post ( sem );

//     }
// }



void SharedMem(int n) {
  int shmfd;


  shmfd = shm_open("/baralh_memory", O_RDWR | O_CREAT, 0666);
  ftruncate(shmfd, n * n * sizeof(sem_t));
  sem = (sem_t *)mmap (NULL, n*n*sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd,0);
  close(shmfd);
  shm_unlink("/baralh_memory");

  shmfd = shm_open("/baralh_memory", O_RDWR | O_CREAT, 0666);
  ftruncate(shmfd, sizeof(double));
  time = (double *)mmap (NULL, sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd,0);
  close(shmfd);
  shm_unlink("/baralh_memory");


  shmfd = shm_open("/baralh_memory", O_RDWR | O_CREAT, 0666);
  ftruncate(shmfd, n * n * sizeof(double));
  sharedC = (double *)mmap (NULL, n*n*sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd,0);
  close(shmfd);
  shm_unlink("/baralh_memory");




}


////////Threaded Block Multiply////////


void shmtest3 (double **a, double **b, double *d, int n) {



  int i, j, k, i0, j0, k0, status, pid, kids, shmfd;
  double start, end; 
  for (i = 0; i<n*n ; i++)
    sem_init(&sem[i], 1, 1);


    //kids = argc > 1 ? atoi(argv[1]) : 5;


    // sem = sem_open ( "baralh_sem", O_CREAT, 0666, 1 );

    // if ( sem == NULL ) {

    //     fprintf(stderr,"Could not create brs semaphore\n");

    //     exit(1);


    // }


    // sem_unlink ( "baralh_sem" );

    // mem[0] = 0;


  for ( i = 0; i <n; i+=blocksize) {

   for(j = 0; j<n;j+=blocksize){

    for(k=0; k<n;k+=blocksize){


      cn++;
     pid = fork();

     if ( pid < 0 ) {

      fprintf(stderr,"fork failed at %d\n",i);
      
      exit(1);

    } else if ( pid > 0 ) {
           // printf("parent: new child is %d\n",pid);
      // int wpid; 
      // while ((wpid = wait(&status)) > 0 );

    } else {


      start = ftime();
    // printf("%10.2f\n", start);

      for(i0=i;i0<min(i+blocksize,n);++i0){


       for(j0=j;j0<min(j+blocksize,n);++j0){


        for(k0=k;k0<min(k+blocksize,n);++k0){

          sem_wait(&sem[i0*n+j0]);
          d[i0*n+j0]=d[i0*n+j0]+a[i0][k0]*b[k0][j0];
          sem_post(&sem[i0*n+j0]);

            //printf("child %d, parent is %d\n",i, getppid());
            //work(i);
            //exit(0);
            // }} 
            //for ( i = 0; i < kids; i++ ) wait(NULL);
            //printf("mem[0] = %d\n",mem[0]);
            //exit (0);


        }
      }
    }

    end = ftime();
   // printf("%10.2f\n", end);
    *time += end - start;
    exit(0);

  }

}

}

}

int wpid; 
while((wpid = wait(NULL))>0);

}



///////End Threaded/////////////

int main (void)
{
 int i, j, n;
 double **a, **b, **c, *d;

 printf ( "Enter the value of n: ");
 scanf ( "%d", &n);

 printf ( "\n");


   //Populate arrays....

 a= (double**)malloc(n*sizeof(double));
 b= (double**)malloc(n*sizeof(double));
 c= (double**)malloc(n*sizeof(double));
 //d= (double*)malloc(n*n*sizeof(double));

 for (i=0; i<n; i++)
 {
   a[i]= malloc(sizeof(double)*n);
   b[i]= malloc(sizeof(double)*n);
   c[i]= malloc(sizeof(double)*n);
 }

 for (i=0; i<n; i++)
 {
  for (j=0; j<n; j++)
   a[i][j]=8;
}

for (i=0; i<n; i++)
{
  for (j=0; j<n; j++)
    b[i][j]=7;
}



      ///////Normal/////////
printf("Normal Matrix:\n");

start = ftime();
multiply (a,b,c,n);
stop = ftime();
used = stop - start;
mf = (n*n*n *2.0) / used / 1000000.0;
printf ("\n");
printf ( "Elapsed time:   %10.2f \n", used);
printf ( "DP MFLOPS:       %10.2f \n\n\n", mf);

// printf("=========Normal Matrix=============\n");
// MatrixDisplay(c,n);

      ////////End Normal////////

      ////////Transpose///////

      transpose(b,n); //calling transpose to make B matrix to transpose matrix 

      printf("Transpose Matrix: \n");  

      start = ftime();
      transmultiply (a,b,c,n);
      stop = ftime();
      used = stop - start;
      mf = (n*n*n *2.0) / used / 1000000.0;
      printf ("\n");
      printf ( "Elapsed time:   %10.2f \n", used);
      printf ( "DP MFLOPS:       %10.2f \n\n\n", mf);

      // printf("=========Transpose Matrix==========\n");

      // MatrixDisplay(c,n);

      ////////End Trans//////


      ////////Block//////////

      transpose(b,n); //calling transpose to make B matrix normal


      //block(a, b, c, n);
      printf("Block Matrix:\n");

      start = ftime();
      block(a,b,c,n);
      stop = ftime();
      used = stop - start;
      mf = (n*n*n *2.0) / used / 1000000.0;
      printf ("\n");
      printf ( "Elapsed time:   %10.2f \n", used);
      printf ( "DP MFLOPS:       %10.2f \n\n\n", mf);

        // printf("=========Block Matrix==============\n");


        // printf("Block Matrix:\n"); 

        // MatrixDisplay(c,n);

      ////////End Block//////////


      ///////Threaded///////////

      printf("Fork Matrix:\n");
      SharedMem(n);
     // start = ftime();
      shmtest3(a,b,sharedC,n);
      //stop = ftime();
      used = *time/cn;
      mf = (n*n*n *2.0) / used / 1000000.0;
      printf ("\n");
      printf ( "Elapsed time:   %10.2f \n", used);
      printf ( "DP MFLOPS:       %10.2f \n\n\n", mf);

     // printf("=========Fork Matrix==============\n");

      
     // printf("Block Matrix:\n"); 

     // MatrixDisplay(c,n);



      /////Threaded End/////////

      return (0);
    }

