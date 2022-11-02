#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
	pid_t pid1,pid2;
	//3. printf("Out put a sentence without it");
	//3. printf("Out put a sentence with it\n");
	while ((pid1=fork())==-1);  //Error!
	if (pid1==0){  //child1 process
		//1. printf("b");  //Output "b"
		lockf(1,1,0);
		for (int i=0;i<3;i++){
			printf("I'm child1 process\n");
		}
		lockf(1,0,0);
	}else{
		while ((pid2=fork())==-1);  //Error!
		if (pid2==0){  //child2 process
			//1. printf("c");
			lockf(1,1,0);
			for (int i=0;i<3;i++){
				printf("I'm child2 process\n");
			}
			lockf(1,0,0);
		}else{
			lockf(1,1,0);
			for (int i=0;i<3;i++){
				printf("I'm father process\n");
			}
			lockf(1,0,0);
		}
	}
	return 0;
}
a
