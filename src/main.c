#include<stdio.h>
#include<math.h>

#include<sys/time.h>
#include<unistd.h>
#include<stdlib.h>

void Usage(char* prog_name);

long thread_count;
long long iteration;
double sum;
const double micro = 1.0e-6;

double calculateTime()
{
struct timeval TV;
struct timezone TZ;
int RC = gettimeofday(&TV,&TZ);
if (RC == -1){
printf("Bad call to gettimeofday\n");
return(-1);
}
return ( ((double)TV.tv_sec ) + micro * ((double) TV.tv_usec
));
}
void Get_args(int argc, char* argv[]) {
   if (argc != 3) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);  
   if (thread_count <= 0) Usage(argv[0]);
    iteration = strtoll(argv[2], NULL, 10);
   if (iteration <= 0) Usage(argv[0]);
}  /* Get_args */
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads> <n>\n", prog_name);
   fprintf(stderr, "   n is the number of terms and should be >= 1\n");
   fprintf(stderr, "   n should be evenly divisible by the number of threads\n");
   exit(0);
}  /* Usage */
int main(int argc, char* argv[])
{
    Get_args(argc, argv);

    double start,finish;
    start = calculateTime();
    double h = 2.0/iteration;
    double sum = 0.0,x;
    for(int i = 1; i < iteration; i++)
    {
        x = -1 + (i + 0.5f) * h;
        sum +=sqrt(1.0-x*x)*h;
    }

    double pi = 2 *sum;
    finish = calculateTime();


    printf(" pi is %.15lf \n", pi);
    printf(" time taken = %lf",finish - start);
}