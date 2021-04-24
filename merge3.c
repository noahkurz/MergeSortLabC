// C program to implement concurrent merge sort
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void insertionSort(int arr[], int n);
//void merge(int a[], int l1, int h1, int h2);

void mergeSort(int a[], int l)
{
    int halfWayPoint = l / 2;
    pid_t lpid, rpid;
    lpid = fork();
    sleep(1);
    if (lpid < 0)
    {
        // Lchild proc not created
        perror("error forking left child\n");
        _exit(-1);
    }
    else if (lpid == 0)
    {
        sleep(1);
        //we are in the child
        printf("left child is currently looking at...\n");
        for(int i = 0; i <= halfWayPoint; i++)
        {
            printf(" %d,",a[i]);
        }
        printf("\n");
        if(l > 1)
    {
        mergeSort(a, halfWayPoint);
    }
    else{
        printf("sorted\n");
        exit(0);
    }
        sleep(2);


        sleep(1);
        _exit(0);
    }
    else
    {
        rpid = fork();
        if (rpid < 0)
        {
            // Rchild proc not created
            perror("Right Child Proc. not created\n");
            exit(-1);
        }
        else if (rpid == 0)
        {
        printf("right child is currently looking at...\n");
        for(int i = halfWayPoint; i > 0; i--)
        {
            printf(" %d,",a[i]);
        }
        if(l > 1)
        {
            mergeSort(a, halfWayPoint+1);
        }
        else
        {
            printf("sorted!!\n");
            exit(0);
        }
        
        printf("\n");
        sleep(1);
        exit(0);
        }
    }

    int status;

    // Wait for child processes to finish
    waitpid(lpid, &status, 0);
    waitpid(rpid, &status, 0);

}



// To fill randome values in array for testing
// purpise
void fillData(int a[])
{
    // Create random arrays
    for (int i = 0; i <= 10; i++)
    {
        a[i] = (rand() % 300);
        printf("At bucket %d we have the random value %d \n", i, a[i]);
    }
}

// Driver code
int main()
{
    int shmid;
    key_t key = IPC_PRIVATE;
    int *shm_array;

    // Using fixed size array.  We can uncomment
    // below lines to take size from user

    /* printf("Enter No of elements of Array:");
    scanf("%d",&length); */

    // Calculate segment length
    size_t SHM_SIZE = sizeof(int) * 10;

    // Create the segment.
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        _exit(1);
    }

    // Now we attach the segment to our data space.
    if ((shm_array = shmat(shmid, NULL, 0)) == (int *)-1)
    {
        perror("shmat");
        _exit(1);
    }

    // Create a random array of given length
    srand(time(NULL));
    fillData(shm_array);

    // Sort the created array
    mergeSort(shm_array, 10);
    return 0;
}