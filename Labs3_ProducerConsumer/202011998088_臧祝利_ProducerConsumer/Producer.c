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
#define SEM_SIZE 1024*(N+1)

struct Memory{
    char memory[N][BUFFERSIZE];
    int in, out;
};

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *_buf;
};

int P(int semid, int semnum){
    struct sembuf sops={semnum,-1,0};
    return (semop(semid,&sops,1));
}

int V(int semid, int semnum){
    struct sembuf sops={semnum,+1,0};
    return (semop(semid,&sops,1));
}

int main(){
    int key,semid,ret,shmid;
    int mutex,full,empty;
    struct Memory *sharing;
    union semun arg;
    void *shm = NULL;
    char msg[BUFFERSIZE];
    key = ftok("./consumer.c",0);
    if (key == -1){
        printf("ftok() failed\n");
        exit(EXIT_FAILURE);
    }

    semid = semget(key,SEM_NUM,0660|IPC_CREAT|IPC_EXCL);
    if (semid == -1){
        if (errno == EEXIST){
            printf("semget() warning: %s\n", strerror(errno));
            semid = semget(key,0,0660|IPC_CREAT);
            if (semid == -1){
                printf("semget() error: %s\n",strerror(errno));
                return -1;
            }
            printf("exist semget() success. semid=[%d]\n",semid);
        }
        else{
            printf("semget() error: %s\n",strerror(errno));
            return -1;
        }
    }else{
        printf("create semget() success. semid=[%d]\n",semid);
        arg.val = 1;  //init mutex value
        ret = semctl(semid,0,SETVAL,arg);  //mutex
        if (ret < 0){
            perror("ctl sem error");
            semctl(semid,0,IPC_RMID,arg);
            return -1;
        }
        arg.val=0;  //init full value
        ret = semctl(semid,1,SETVAL,arg);  //full
        if (ret<0){
            perror("ctl sem error");
            semctl(semid,0,IPC_RMID,arg);
            return -1;
        }
        arg.val=N;  //init empty value
        ret = semctl(semid,2,SETVAL,arg);  //empty
        if (ret<0){
            perror("ctl sem error");
            semctl(semid,0,IPC_RMID,arg);
            return -1;
        }
    }

    shmid = shmget((key_t)key,SEM_SIZE,0660|IPC_CREAT|IPC_EXCL);
    if (shmid == -1){
        shmid = shmget(key,SEM_SIZE,0660|IPC_CREAT);
        if (shmid==-1){
            printf("shmget() failed\n");
            exit(EXIT_FAILURE);
        }else{
            printf("exist shmget() success. shmid=[%d]\n",shmid);
            shm = shmat(shmid,NULL,0);
            sharing = (struct Memory *)shm;
        }
    }else{
        printf("create shmget() success. shmid=[%d]\n",shmid);
        shm = shmat(shmid,NULL,0);
        sharing = (struct Memory *)shm;
        for (int i=0;i<N;i++){
            strcpy(sharing->memory[i],"");
        }
        sharing->in = 0;
        sharing->out = 0;
    }

    while (1){
        printf("Please choose one option:\n");
        printf("1. Produce a product; 2. Exit; 3. Delete Sharing Momery and exit\n");
        int option;
        scanf("%d",&option);
        if (semid==-1||mutex==-1){
            printf("Sharing memory error!\n");
            exit(0);
        }
        if (option==1){
            mutex = semctl(semid,0,GETVAL,arg);
            full = semctl(semid,1,GETVAL,arg);
            empty = semctl(semid,2,GETVAL,arg);
            printf("product before. semval[0](mutex)=[%d] semval[1](full)=[%d] semval[2](empty)=[%d]\n",mutex,full,empty);
            printf("before-----in: %d, out: %d\n",sharing->in,sharing->out);
            if (full == N){
                printf("Please wait consumer...\n");
            }
            while ((full=semctl(semid,1,GETVAL,arg))==10);
            printf("Input your message:");
            scanf("%s",msg);
            P(semid,2);  //wait(empty);
            P(semid,0);  //wait(mutex);

            strcpy(sharing->memory[sharing->in],msg);
            sharing->in = (sharing->in + 1)%N;
            printf("Products:%s\n",msg);
            V(semid,0);  //signal(mutex);
            V(semid,1);  //signal(full);

            mutex = semctl(semid,0,GETVAL,arg);
            full = semctl(semid,1,GETVAL,arg);
            empty = semctl(semid,2,GETVAL,arg);
            printf("product finish. semval[0](mutex)=[%d] semval[1](full)=[%d] semval[2](empty)=[%d]\n",mutex,full,empty);
            printf("product in=[%d]\n",sharing->in);
            printf("after-----in: %d, out: %d\n",sharing->in,sharing->out);
        }else if (option==2){
            shmdt(sharing);
            printf("Exit from the process\n");
            exit(0);

        }else{
            shmdt(sharing);
            semctl(semid,IPC_RMID,0);
            shmctl(shmid,IPC_RMID,0);
            printf("Delete the sharing memory\n");
            exit(1);
        }
    }
    return 0;
}