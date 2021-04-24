#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

int numberOfBuckets = 11;
int randNums[11];
int childNum;

void childSortedHandler(int signum)
{
    
    printf("my array is sorted PID: %d\nContents of my memory are... %d\n", getpid(), childNum);
    sleep(1);
}

int main()
{

    for (int i = 0; i <= 10; i++)
    {
        randNums[i] = (rand() % 300);
        printf("At bucket %d we have the random value %d \n", i, randNums[i]);
    }
    time_t t;
    srand((unsigned)time(&t));
    //printf("I am PID: %d \n", getpid());

    for (int i = 0; i <= 11; i++)
    {
        sleep(2);
        pid_t pid = fork();
        childNum = randNums[i];
        if (pid < 0)
        {
            printf("Fork Failed\n");
            exit(1);
        }
        else if (pid == 0) //This is the child process
        {
            signal(SIGUSR2, childSortedHandler);
            pause();
            kill(pid, SIGUSR2);
        }
        else //This will be the adult
        {
            signal(SIGUSR2, childSortedHandler);
            //printf("%d \n", pid);
            kill(pid, SIGUSR2);

            //pause();
        }
    }
}
