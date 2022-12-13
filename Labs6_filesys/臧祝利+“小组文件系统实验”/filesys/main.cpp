#include <stdio.h>
#include "filesys.h" 

struct hinode 	hinode[NHINO];

struct dir 		dir;
struct file 	sys_ofile[SYSOPENFILE];
struct filsys 	filsys;
struct pwd 		pwd[PWDNUM];
struct user 	user[USERNUM];
struct inode* 	cur_path_inode;
int 			user_id;
char   			disk[(DINODEBLK+FILEBLK+2)*BLOCKSIZ];
char 			str[100];
char* address_name[100];
int point_address;
int usernumber;  //当前用户数量

int main(){
	int username;
	char password[16];
	user_id = -1;
	int op;
	FILE* fp;
	point_address = 1;
	for (int i = 0; i < 100; i++) {
		address_name[i] = (char*)malloc(sizeof(char) * 100);
	}
	
	strcpy(address_name[0],"root"); //初始目录命名为root

	printf("Welcome to mini filesystem!\n");
	printf("Enter the username you will operator! ,make sure you won't change it when you log in\n");
	scanf("%d", &username);
	char txtname[100];
	itoa(username, txtname, 10);
	strcat(txtname, ".txt");
	char filename[100] = "user/";
	strcat(filename, txtname);
	do {
		printf("Please enter your choice!\n");
		printf("[0] New Filesys; [1] Last Filesys\n");
		scanf("%d", &op);
		if (op == 0) {
			format();
		}
		else if (op == 1) {
			fp = fopen(filename, "rb");
			if (fp == NULL) {
				printf("此用户本次为第一次操作！无法获取上一次操作\n");
				format();
				break;
			}
			memset(disk, 0x00, ((DINODEBLK + FILEBLK + 2) * BLOCKSIZ));
			fread(disk, sizeof(char), (DINODEBLK + FILEBLK + 2) * BLOCKSIZ, fp);
			memcpy(pwd, disk + DATASTART + BLOCKSIZ * 2, BLOCKSIZ);
			fclose(fp);
		}
		else {
			printf("Please enter the choice again!\n");
		}
	} while (op != 0 && op != 1);
    //format();
	install();
	while (user_id == -1) {
		printf("Login:");
		scanf("%d", &username);
		printf("Password:");
		scanf("%s", password);
		user_id = login(username, password);
	}
	do{
		printf("%d@filesys:", user[user_id].u_uid);
		for (int i = 0;i < point_address; i++) {
			printf("/%s", address_name[i]);
		}
		printf("> ");
		rewind(stdin);
		gets_s(str);
	}while(shell(user_id,str));
	logout(username);
	halt(username);
	return 0;
}




