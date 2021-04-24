#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <string.h>

void rightArraySplitter(int arrayToSplit[], int numberOfBuckets);
void leftArraySplitter(int arrayToSplit[], int numberOfBuckets);
int numberOfBuckets = 10;
int randNums[10];
int childNum[10];

void childSortedHandler(int signum)
{

    printf("my array is sorted PID: %d\n\n", getpid());
    sleep(1);
    exit(0); //cheap trick to kill
}

int main()
{
    //OG array
    for (int i = 0; i <= 10; i++)
    {
        randNums[i] = (rand() % 300);
        printf("At bucket %d we have the random value %d \n", i, randNums[i]);
    }
    //time_t t;
    //srand((unsigned)time(&t));
    //memcpy(r, randNums, (numberOfBuckets / 2) + 1 * sizeof(int));
    leftArraySplitter(randNums, (numberOfBuckets / 2));
    rightArraySplitter(randNums, (numberOfBuckets / 2) + 1);
    signal(SIGUSR2, childSortedHandler);
    pause();

}
void leftArraySplitter(int arrayToSplit[], int numberOfBuckets)
{
    
    if (numberOfBuckets != 1)
    {
        printf("number of buckets is... %d", numberOfBuckets);
         printf("we made a left baby\n");
        for (int i = 0; i <= (numberOfBuckets / 2); i++)
        {
            printf("  %d,", arrayToSplit[i]);

        }
        printf("\n\n");

        printf("\n");
        int l[numberOfBuckets / 2];
        sleep(2);
        pid_t pid = fork();
        if (pid < 0)
        {
            printf("Fork Failed\n");
            exit(1);
        }
        else if (pid == 0) //This is the child process
        {
            //memcpy(l, arrayToSplit, (numberOfBuckets / 2) * sizeof(int));
            for (int i = 0; i <= numberOfBuckets; i++)
            {
                l[i] = arrayToSplit[i];


            }
            leftArraySplitter(l, (numberOfBuckets / 2));
            rightArraySplitter(l, (numberOfBuckets / 2 + 1));
            signal(SIGUSR2, childSortedHandler);
            pause();
            
        }
        else //This will be the adult
        {
            signal(SIGUSR2, childSortedHandler);
            //printf("%d \n", pid);

            //pause();
        }
    }
    else
    {
        kill(getpid(), SIGUSR2);
        printf("we made it to the one bucket");
    }

}
void rightArraySplitter(int arrayToSplit[], int numberOfBuckets)
{
    if (numberOfBuckets != 1)
    {
        printf("we made a right baby\n");
        for (int i = 0; i <= numberOfBuckets; i++)
        {
            printf("  %d,", arrayToSplit[i + numberOfBuckets]);

        }

        printf("\n\n");

        int r[numberOfBuckets];
        sleep(2);
        pid_t pid = fork();
        if (pid < 0)
        {
            printf("Fork Failed\n");
            exit(1);
        }
        else if (pid == 0) //This is the child process
        {
            //memcpy(r, arrayToSplit, (numberOfBuckets / 2) + 1 * sizeof(int));
            for (int i = 0; i <= numberOfBuckets; i++)
            {
                if (randNums[numberOfBuckets + 1] == 0)
                {
                    printf("we reached the end of the array\n");
                    //kill(getpid(), SIGUSR2);
                }
                else
                {
                r[i] = arrayToSplit[i + numberOfBuckets];
                }
            }
            leftArraySplitter(r, (numberOfBuckets / 2));
            rightArraySplitter(r, (numberOfBuckets / 2 + 1));
            signal(SIGUSR2, childSortedHandler);
            pause();
        }
        else //This will be the adult
        {
            signal(SIGUSR2, childSortedHandler);
            //printf("%d \n", pid);

            //pause();
        }
    }
    else
    {
        kill(getpid(), SIGUSR2);
        printf("we made it to the one bucket");
    }
}




//check to see how big the array is
//if the array is of size 1 we are sorted call a handler that says this
//if we are an array of size 1 plus
//find the mid point and split the array in half
//loop hrough assign the first half to the temp array "l" fork the process
//and start over with this new process and the new array
//loop through and assign the REST to temp array "r" fork the process
//and start over with this new process and the new array
//when array is of size 1 then call send a smoke signal up and say "I am sorted"
