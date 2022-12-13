#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesys.h" 

int getusernumber() {
	FILE* fp;
	fp = fopen("count.txt", "r");
	if (fp == NULL) {
		printf("not found!\n");
	}
	fscanf(fp, "%d", &usernumber);
	//printf("usernumber_now = %d\n", usernumber);
	fclose(fp);
	return usernumber;
}

void storeusernumber() {
	FILE* fp;
	fp = fopen("count.txt","w");
	fprintf(fp, "%d",usernumber);
	fclose(fp);
}
void adduser(int username,char* pswd,int group) {
	if (username == 2116 || username == 2117 || username == 2118 || username == 2119 || username == 2120) {
		printf("Sorry ,the user existed\n");
		return;
	}
	usernumber = getusernumber();
	for (int i = 0;i < usernumber;i++) {
		if (username == pwd[i].p_uid) {
			printf("Sorry ,the user existed\n");
			return;
		}
	}
	if (usernumber == 32) {
		printf("用户数量已达上限(32)!\n");
		return;
	}
	char txtname[100];  //文件名称
	itoa(usernumber, txtname, 10);  //将数字转换成字符串
	strcat(txtname, ".txt");  //生成文件后缀,例如5.txt,用于之后赋值pwd
	char filename[100] = "user_info/";  //增加目录名
	strcat(filename, txtname);  //完整文件路径
	FILE* fp;
	fp = fopen(filename, "wb");  //打开文件
	fprintf(fp, "%d\n%s\n%d\n", username, pswd, group);
	fclose(fp);
	printf("用户创建成功!\n");
	usernumber++;
	storeusernumber();
	return;
}

void modify_pswd(int username, char* new_pswd) {
	usernumber = getusernumber();
	for (int i = 0;i < usernumber;i++) {
		if (username == pwd[i].p_uid) {  //找到用户了,开始修改
			char pswd[100];

			char txtname[100];  //文件名称
			itoa(i, txtname, 10);  //将数字转换成字符串
			strcat(txtname, ".txt");  //生成文件后缀,例如5.txt,用于之后赋值pwd
			char filename[100] = "user_info/";  //增加目录名
			strcat(filename, txtname);  //完整文件路径
			FILE* fp;
			fp = fopen(filename, "wb");  //打开文件
			fprintf(fp, "%d\n%s\n%d\n", username, new_pswd, pwd[i].p_gid);
			fclose(fp);
			printf("修改成功!\n");
			return;
		}
	}
	printf("Not found the user!\n");
	return;
}

void helporder(char* order) {
	if (strcmp(order,"exit")==0) {
		printf("退出程序并且将当前用户的磁盘存储到文件中\n");
	}
	else if (strcmp(order, "dir") == 0) {
		printf("显示当前目录\n");
	}
	else if (strcmp(order, "cd") == 0) {
		printf("切换目录\n");
		printf("cd [dirname]\n");
	}
	else if (strcmp(order, "mkfile") == 0) {
		printf("创建新文件\n");
		printf("mkfile [filename]\n");
	}
	else if (strcmp(order, "write") == 0) {
		printf("写入文件\n");
		printf("write [filename] [size]\n");
	}
	else if (strcmp(order, "read") == 0) {
		printf("读取文件\n");
		printf("read [filename] [size]\n");
	}
	else if (strcmp(order, "adduser") == 0) {
		printf("增加新用户\n");
		printf("adduser [username] [password] [group_id]\n");
	}
	else if (strcmp(order, "pwd") == 0) {
		printf("显示当前路径\n");
	}
	else if (strcmp(order, "who") == 0) {
		printf("显示当前用户信息\n");
	}
	else if (strcmp(order, "password") == 0) {
		printf("修改用户密码\n");
		printf("password [username] [new_password]\n");
	}
	return;
}