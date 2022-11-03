#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define N 10
#define BUFFERSIZE 1024
#define SEM_NUM 3
#define SEM_SIZE 1024 * (N + 1)

struct Memory
{
    char memory[N][BUFFERSIZE];
    int in, out;
};

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *_buf;
};

int P(int semid, int semnum)
{
    struct sembuf sops = {semnum, -1, 0};
    return (semop(semid, &sops, 1));
}

int V(int semid, int semnum)
{
    struct sembuf sops = {semnum, +1, 0};
    return (semop(semid, &sops, 1));
}

int main()
{
    int key, semid, ret, shmid;
    int mutex, full, empty;
    struct Memory *sharing;
    union semun arg;
    void *shm = NULL;
    char message[BUFFERSIZE];
    key = ftok("./consumer.c", 0);
    if (key == -1)
    {
        printf("ftok() failed\n");
        exit(EXIT_FAILURE);
    }

    semid = semget(key, SEM_NUM, 0660 | IPC_CREAT);
    if (semid == -1)
    {
        perror("create semget error");
        return -1;
    }
    else
    {
        printf("semget() success. semid=[%d]\n", semid);
    }

    if ((shmid = shmget(key, SEM_SIZE, 0666 | IPC_CREAT | IPC_EXCL)) != -1)
    {
        printf("Please create shared memory\n");
        semctl(semid, 0, IPC_RMID, 0);
        shmctl(shmid, IPC_RMID, 0);
        return -1;
    }
    shmid = shmget(key, SEM_SIZE, 0660 | IPC_CREAT);
    if (shmid == -1)
    {
        printf("shmget() failed\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("shmget() success. shmid=[%d]\n", shmid);
        shm = shmat(shmid, NULL, 0);
        sharing = (struct Memory *)shm;
    }

    while (1)
    {
        printf("Please choose one option:\n");
        printf("1. Consume a product; 2. Exit; 3. Delete Sharing Momery and exit\n");
        int option;
        scanf("%d", &option);
        if (semid == -1 || mutex == -1)
        {
            printf("Sharing memory error!\n");
            exit(0);
        }
        if (option == 1)
        {
            mutex = semctl(semid, 0, GETVAL, arg);
            full = semctl(semid, 1, GETVAL, arg);
            empty = semctl(semid, 2, GETVAL, arg);
            printf("consume before. semval[0](mutex)=[%d] semval[1](full)=[%d] semval[2](empty)=[%d]\n", mutex, full, empty);
            printf("before-----in: %d, out: %d\n", sharing->in, sharing->out);
            if (full == 0)
            {
                printf("Please wait producer...\n");
            }
            while ((full = semctl(semid, 1, GETVAL, arg)) == 0)
                ;

            P(semid, 1); // wait(full);
            P(semid, 0); // wait(mutex);

            strcpy(sharing->memory[sharing->out], message);
            sharing->out = (sharing->out + 1) % N;

            V(semid, 0); // signal(mutex);
            V(semid, 2); // signal(empty);

            mutex = semctl(semid, 0, GETVAL, arg);
            full = semctl(semid, 1, GETVAL, arg);
            empty = semctl(semid, 2, GETVAL, arg);
            printf("consume finish. semval[0](mutex)=[%d] semval[1](full)=[%d] semval[2](empty)=[%d]\n", mutex, full, empty);
            printf("product out=[%d]\n",sharing->out);
            printf("after-----in: %d, out: %d\n", sharing->in, sharing->out);
        }
        else if (option == 2)
        {
            shmdt(sharing);
            printf("Exit from the process\n");
            exit(0);
        }
        else
        {
            shmdt(sharing);
            semctl(semid, IPC_RMID, 0);
            shmctl(shmid, IPC_RMID, 0);
            printf("Delete the sharing memory\n");
            exit(1);
        }
    }
    return 0;
}