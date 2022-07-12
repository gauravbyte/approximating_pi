#include<stdio.h>
#include<math.h>
#include<pthread.h>
#include<sys/time.h>
#include<unistd.h>
#include<stdlib.h>


long thread_count;
long long iteration;
double sum;
const double micro = 1.0e-6;
double GLOBAL_PI = 0.0;
double times;
double timep;
pthread_mutex_t mutex;

void Usage(char* prog_name);

void Get_args(int argc, char* argv[]) {
   if (argc != 3) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);  
   if (thread_count <= 0) Usage(argv[0]);
    iteration = strtoll(argv[2], NULL, 10);
   if (iteration <= 0) Usage(argv[0]);
}  /* Get_args */

double calculateTime()
{
struct timeval TV;
struct timezone TZ;
int RC = gettimeofday(&TV,&TZ);
if (RC == -1){
printf("Bad call to gettimeofday\n");
return(-1);
}
return ( ((double)TV.tv_sec ) + micro * ((double) TV.tv_usec));
}

void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads> <n>\n", prog_name);
   fprintf(stderr, "   n is the number of terms and should be >= 1\n");
   fprintf(stderr, "   n should be evenly divisible by the number of threads\n");
   exit(0);
}  /* Usage */
void serial_integral()
{
    long long int i;
    double start,finish;
    start = calculateTime();
    double h = 2.0/iteration;
    double sum = 0.0,a;
    for( i = 1; i < iteration; i++)
    {
        a = -1 + (i + 0.5f) * h;
        sum +=sqrt(1.0-a*a)*h;
    }

    double pi = 2 *sum;
    finish = calculateTime();

    printf("serial execution \n");
    printf("pi calculated = %.15lf \n", pi);
    printf("time taken    = %lf \n",finish - start);
    times = finish-start;
}
void *Parellel_integral(void *pArg){
           int thread_array = *((int *)pArg);
           double h = 2.0 / iteration;
           double sum_of_thread = 0.0, a;  
           for (int i = thread_array; i < iteration; i += thread_count){
                  a = -1 + (i + 0.5f) * h;
                   sum_of_thread += sqrt(1.0 - a*a) * h;
           }
           pthread_mutex_lock(&mutex);
           GLOBAL_PI += sum_of_thread;
           pthread_mutex_unlock(&mutex);
           return 0;
}
int main(int argc, char* argv[])
{
    printf("accurate pi 3 . 1 4 1 5 9 2 6 5 3 5 8 9 7 9 3 \n");
    Get_args(argc, argv);
    serial_integral();


    printf("parellel execution \n");
    double start,finish;
    pthread_t threadids[thread_count]; int thread_array[thread_count], i;
    pthread_mutex_init(&mutex, NULL);
    start = calculateTime();
    for ( i = 0; i < thread_count; ++i ) 
    {
    thread_array[i] = i;
    pthread_create(&threadids[i], NULL, Parellel_integral, (void*)&thread_array[i]);
    }
    for ( i = 0; i < thread_count; ++i )
    {
    pthread_join(threadids[i], NULL);
    }
    GLOBAL_PI *= 2.0;
    finish = calculateTime();
    printf("pi calculated = %.15f   \n", GLOBAL_PI );
    printf("time taken    = %lf \n",finish-start);
    timep = finish - start;
    printf("speed up %lf \n",times/timep);
    pthread_mutex_destroy(&mutex);
    return 0;
    

}