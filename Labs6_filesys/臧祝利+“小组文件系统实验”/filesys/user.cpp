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
		printf("�û������Ѵ�����(32)!\n");
		return;
	}
	char txtname[100];  //�ļ�����
	itoa(usernumber, txtname, 10);  //������ת�����ַ���
	strcat(txtname, ".txt");  //�����ļ���׺,����5.txt,����֮��ֵpwd
	char filename[100] = "user_info/";  //����Ŀ¼��
	strcat(filename, txtname);  //�����ļ�·��
	FILE* fp;
	fp = fopen(filename, "wb");  //���ļ�
	fprintf(fp, "%d\n%s\n%d\n", username, pswd, group);
	fclose(fp);
	printf("�û������ɹ�!\n");
	usernumber++;
	storeusernumber();
	return;
}

void modify_pswd(int username, char* new_pswd) {
	usernumber = getusernumber();
	for (int i = 0;i < usernumber;i++) {
		if (username == pwd[i].p_uid) {  //�ҵ��û���,��ʼ�޸�
			char pswd[100];

			char txtname[100];  //�ļ�����
			itoa(i, txtname, 10);  //������ת�����ַ���
			strcat(txtname, ".txt");  //�����ļ���׺,����5.txt,����֮��ֵpwd
			char filename[100] = "user_info/";  //����Ŀ¼��
			strcat(filename, txtname);  //�����ļ�·��
			FILE* fp;
			fp = fopen(filename, "wb");  //���ļ�
			fprintf(fp, "%d\n%s\n%d\n", username, new_pswd, pwd[i].p_gid);
			fclose(fp);
			printf("�޸ĳɹ�!\n");
			return;
		}
	}
	printf("Not found the user!\n");
	return;
}

void helporder(char* order) {
	if (strcmp(order,"exit")==0) {
		printf("�˳������ҽ���ǰ�û��Ĵ��̴洢���ļ���\n");
	}
	else if (strcmp(order, "dir") == 0) {
		printf("��ʾ��ǰĿ¼\n");
	}
	else if (strcmp(order, "cd") == 0) {
		printf("�л�Ŀ¼\n");
		printf("cd [dirname]\n");
	}
	else if (strcmp(order, "mkfile") == 0) {
		printf("�������ļ�\n");
		printf("mkfile [filename]\n");
	}
	else if (strcmp(order, "write") == 0) {
		printf("д���ļ�\n");
		printf("write [filename] [size]\n");
	}
	else if (strcmp(order, "read") == 0) {
		printf("��ȡ�ļ�\n");
		printf("read [filename] [size]\n");
	}
	else if (strcmp(order, "adduser") == 0) {
		printf("�������û�\n");
		printf("adduser [username] [password] [group_id]\n");
	}
	else if (strcmp(order, "pwd") == 0) {
		printf("��ʾ��ǰ·��\n");
	}
	else if (strcmp(order, "who") == 0) {
		printf("��ʾ��ǰ�û���Ϣ\n");
	}
	else if (strcmp(order, "password") == 0) {
		printf("�޸��û�����\n");
		printf("password [username] [new_password]\n");
	}
	return;
}