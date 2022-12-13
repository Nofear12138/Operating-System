#include "filesys.h"
#define CLEN 20
#define CNUM 20
//enum ctype  
char commands[CNUM][CLEN]={
"exit",
"dir",
"mkdir",
"cd",
"mkfile",
"del",
"write",
"read",
"adduser",
"pwd",
"who",
"password",
"help"
};
int getcid(char *command){
	int i;
	if(command == NULL)
		return -1;
	for(i=0;i<CNUM;i++){
		if(strcmp(command,commands[i]) == 0)
			return i;
	}
	return -1;
}
int shell(int user_id,char *str){
	char seps[] =" \t\n\0";
	char* token,*tstr,*buf;
	unsigned short mode;
	int fd;
	int cid,size;
	token = strtok(str,seps);
	if(token == NULL)
		return 1;
	cid = getcid(token);
	switch(cid){
	case 1:
		_dir();
		break;
	case 2:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("mkdir�������ȷ��ʽΪmkdir dirname����������!\n");
			break;
		}
		mkdir(token);
		break;
	case 3:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("cd�������ȷ��ʽΪcd dirname����������!\n");
			break;
		}
		if (strcmp(token, "..") && chdir(token)!=-1) {  //��������ϼ�Ŀ¼����Ŀ¼����
			strcpy(address_name[point_address], token);  //����ջ��
			point_address++;
		}
		else if (strcmp(token, "..") == 0) {  //���򷵻���һ��Ŀ¼
			point_address--;
		}
		break;
	case 4:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("mkfile �������ȷ��ʽΪmkfile filename [mode]����������!\n");
			break;
		}
		tstr =token;
		mode = DEFAULTMODE;
		token = strtok(NULL,seps);
		if(token != NULL){
			sscanf(token,"%o",&mode);
		}
		mode = mode|DIFILE|0700;
		fd = creat(user_id,tstr,mode);
		if(fd == -1){
			printf("�����ļ�ʧ�ܣ�\n");
			break;
		}
		close(user_id,fd);
		break;
	case 5:
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("del �������ȷ��ʽΪdel filename����������!\n");
			break;
		}
		delete(token);
		break;
	case 6:  //wirte
		mode = WRITE;
		token = strtok(NULL,seps);
		tstr = token;
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("write �������ȷ��ʽΪwrite filename bytes����������!\n");
			break;
		}
		if(token[0] == '-'){
			if(token[1] == 'a')
				mode = FAPPEND;
		}else{
			sscanf(token,"%d",&size);
		}
		fd = open(user_id,tstr,char(mode));
		if (fd == -1) {
			printf("unable to write!\n");
			break;
		}

		buf = (char*)malloc(size);
		size = write(fd,buf,size);
		printf("%d bytes have been writed in file %s.\n",size,tstr);
		free(buf);
		close(user_id,fd);
		break;
	case 7:
		token = strtok(NULL,seps);
		tstr = token;
		token = strtok(NULL,seps);
		if(token == NULL){
			printf("read �������ȷ��ʽΪread filename bytes����������!\n");
			break;
		}
		sscanf(token,"%d",&size);
		fd = open(user_id,tstr,READ);
		if (fd == -1) {
			printf("unable to read\n");
			break;
		}
		buf = (char*)malloc(size+1);
		size = read(fd,buf,size);
		printf("%d bytes have been read in buf from file %s.\n",size,tstr);
		free(buf);
		close(user_id,fd);
		break;

	case 8:  //adduser
		int id, group;
		char* ids, * groups;
		char* pswd;
		token = strtok(NULL, seps);
		ids = token;
		token = strtok(NULL, seps);
		pswd = token;
		token = strtok(NULL, seps);
		if (token == NULL) {
			printf("adduser �������ȷ��ʽΪadduser username password group\n");
			break;
		}
		groups = token;
		id = atoi(ids);
		group = atoi(groups);
		//printf("%d %d\n", id, group);
		adduser(id, pswd, group);
		break;

	case 9:  //pwd����
		for (int i = 0; i < point_address - 1;i++) {
			printf("%s/", address_name[i]);
		}
		printf("%s\n", address_name[point_address - 1]);
		break;

	case 10:
		printf("id\t\tpswd\t\tgroup\n");
		printf("%d\t\t", pwd[user_id].p_uid);
		printf("%s\t\t", pwd[user_id].password);
		printf("%d\n", pwd[user_id].p_gid);
		//printf("��ʱ���һ��ȫ����Ա��username :\n");
		//usernumber = getusernumber();
		//for (int i = 0; i < usernumber;i++) {
			//printf("%d\t\t %s\n", pwd[i].p_uid,pwd[i].password);
		//}
		break;

	case 11:  //password�������޸�����
		char* password, * idss;
		int usid;
		token = strtok(NULL, seps);
		idss = token;
		token = strtok(NULL, seps);
		if (token == NULL) {
			printf("password �������ȷ��ʽΪpassword id new_password����������!\n");
			break;
		}
		password = token;
		usid = atoi(idss);
		modify_pswd(usid, password);

		break;

	case 12:
		token = strtok(NULL, seps);
		if (token == NULL) {
			printf("�й�ĳ���������ϸ��Ϣ������� help ������\n");
			printf("exit\t\t�˳������ҽ���ǰ�û��Ĵ��̴洢���ļ���\n");
			printf("dir\t\t��ʾ��ǰĿ¼\n");
			printf("mkdir\t\t������Ŀ¼\n");
			printf("cd\t\t�л�Ŀ¼\n");
			printf("mkfile\t\t�������ļ�\n");
			printf("del\t\tɾ���ļ�\n");
			printf("write\t\tд���ļ�\n");
			printf("read\t\t��ȡ�ļ�\n");
			printf("adduser\t\t�������û�\n");
			printf("pwd\t\t��ʾ��ǰ·��\n");
			printf("who\t\t��ʾ��ǰ�û���Ϣ\n");
			printf("password\t\t�޸��û�����\n");
			break;
		}
		helporder(token);
		break;

	case 0:
		return 0;
	default:
		printf("����:û������%s��\n",token);
		break;
	}
	return 1;
}
