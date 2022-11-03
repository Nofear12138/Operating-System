#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <wait.h>

#define MSGKEY 75

pid_t pid1,pid2;  //child process

int msgid;
struct msgform{
	long mtype;
	char message[1100];
}msg;

void CLIENT(){
	int i;
	msgid = msgget(MSGKEY, 0777);
	for (i=10;i>=1;i--){
		msg.mtype = i;
		sprintf(msg.message,"(Server) received from pid: %d, type:%ld\n",getpid(),msg.mtype);
		msgsnd(msgid, &msg, 1024, 0);
		printf("(Client)sent %ld\n",msg.mtype);
		sleep(1);
		msgrcv(msgid, &msg ,1024, 0, 0);
		printf("%s",msg.message);	
	}
	exit(0);
}

void SERVER(){
	msgid = msgget(MSGKEY, 0777|IPC_CREAT);
	if (msgid==-1){
		perror("Error!");
		return;
	}
	do{
		msgrcv(msgid, &msg, 1024 ,0, 0);
		printf("%s",msg.message);
		sprintf(msg.message,"(Client) received from pid %d, type %ld\n", getpid(), msg.mtype);
		msgsnd(msgid, &msg, 1024 ,0);
		printf("(SERVER)sent\n");
		sleep(1);
	}while (msg.mtype!=1);
	msgctl(msgid, IPC_RMID, 0);
	exit(0);
}

int main(){
	while((pid1 = fork()) == -1);
	if(!pid1){
		SERVER();
	}else{
		while((pid2 = fork()) == -1);
		if(!pid2){
			CLIENT();
		}else wait(0);
	}
	return 0;
}
