#include <pthread.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

struct args{
double first;
double n;
};
double times;
double timep;
const double micro = 1.0e-6;
long thread_count;
long long iteration;
double value=1;
pthread_mutex_t mutex_lock;
void Usage(char* prog_name);
void Get_args(int argc, char* argv[]) {
   if (argc != 3) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);  
   if (thread_count <= 0) Usage(argv[0]);
    iteration = strtoll(argv[2], NULL, 10);
   if (iteration <= 0) Usage(argv[0]);
}
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads> <n>\n", prog_name);
   fprintf(stderr, "n is the number of terms and should be >= 1\n");
   exit(0);
} 

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
void* parellel_wallis(void *input)
{   

    double size=(((struct args*)input)->n)-((struct args*)input)->first;
    size++;
    double first=((struct args*)input)->first;   
    double numerator=1,denominator=1, result=1;
    
    for (double i=first;i<=((struct args*)input)->n;i++)
   {
        numerator=(4*i*i);
        denominator=((2*i)-1)*((2*i)+1);
        result*=(numerator/denominator);
   }
    pthread_mutex_lock(&mutex_lock);
    value*=result;
    pthread_mutex_unlock(&mutex_lock);    
}
double serial_wallis(double n)
{
    double numerator=1,denominator=1, result=1;
    
   for (double i=1;i<=n;i++)
   {
        numerator=(4*i*i);
        denominator=((2*i)-1)*((2*i)+1);
        result *=(numerator/denominator);
       
   }
   return result*2;
}
int main(int argc, char *argv[]){
Get_args(argc,argv);
long long range=iteration/thread_count;
double start,endtime,ttime;

pthread_t threads[thread_count];
struct args arguments[thread_count];
for (int i=0; i<thread_count;i++)
{   
    if(i==thread_count-1)
    {
        arguments[i].first=1+(i*range);
        arguments[i].n=iteration;
    }
    else
    {
        arguments[i].first=1+(i*range);
        arguments[i].n=range+(i*range);
    }

}
start= calculateTime();
for(int i=0; i<thread_count;i++)
{
    pthread_create(threads+i,NULL,parellel_wallis,(void*)&arguments[i]);
}
for (int i=0; i<thread_count;i++)
{
    long retval;
    pthread_join(threads[i],(void*)&retval);

}
endtime=calculateTime();

timep = endtime - start;

printf("parellel execution PI=%.15lf\n",value*2);
printf("TIme taken = %lf\n",timep);
double time1;
start=calculateTime();
double value1=serial_wallis(iteration);
endtime = calculateTime();

times = endtime-start;
printf("serial execution PI=%.15lf\n",value1);
printf("TIme taken = %lf\n",times);
printf("speed up %lf \n",times/timep);
 

return 0;
}